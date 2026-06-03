function [class_id, probabilities] = ecg_predict(dwt_input, stft_input)
%#codegen
% ЭКГ классификация на основе DWT и STFT признаков
%
% Входы:
%   dwt_input   - [8, 125, 120] single
%   stft_input  - [8, 129, 21] single
%
% Выходы:
%   class_id      - uint8 (0-11)
%   probabilities - [12, 1] single

    persistent w
    if isempty(w)
        w = coder.load('D:/ecg_weights_complete.mat');
        % Приведение всех весов к single
        f = fieldnames(w);
        for i = 1:length(f)
            if isnumeric(w.(f{i}))
                w.(f{i}) = single(w.(f{i}));
            end
        end
    end
    
    % Приведение входов к single (на всякий случай)
    dwt_input = single(dwt_input);
    stft_input = single(stft_input);
    
    % ==================== DWT ВЕТВЬ ====================
    % Conv2d: 8 -> 32, kernel 3x7, padding (1,3)
    x = conv2d_dwt(dwt_input, w.dwt_conv_weights, w.dwt_conv_bias);
    x = batchnorm2d(x, w.dwt_bn_weight, w.dwt_bn_bias, w.dwt_bn_mean, w.dwt_bn_var);
    x = relu(x);
    
    % Residual Block 1
    x = residual_block(x, ...
        w.dwt_res_conv1_weights, w.dwt_res_conv1_bias, ...
        w.dwt_res_conv2_weights, w.dwt_res_conv2_bias, ...
        w.dwt_res1_bn_weight, w.dwt_res1_bn_bias, w.dwt_res1_bn_mean, w.dwt_res1_bn_var, ...
        w.dwt_res2_bn_weight, w.dwt_res2_bn_bias, w.dwt_res2_bn_mean, w.dwt_res2_bn_var);
    
    % SE Attention
    x = se_attention(x, w.dwt_se_fc1_weights, w.dwt_se_fc2_weights);
    
    % Global Average Pooling
    feat_dwt = global_avg_pool(x);  % [32, 1]
    
    % ==================== STFT ВЕТВЬ ====================
    % Conv2d: 8 -> 32, kernel 3x3, padding (1,1)
    y = conv2d_stft(stft_input, w.stft_conv_weights, w.stft_conv_bias);
    y = batchnorm2d(y, w.stft_bn_weight, w.stft_bn_bias, w.stft_bn_mean, w.stft_bn_var);
    y = relu(y);
    
    % Residual Block 2
    y = residual_block(y, ...
        w.stft_res_conv1_weights, w.stft_res_conv1_bias, ...
        w.stft_res_conv2_weights, w.stft_res_conv2_bias, ...
        w.stft_res1_bn_weight, w.stft_res1_bn_bias, w.stft_res1_bn_mean, w.stft_res1_bn_var, ...
        w.stft_res2_bn_weight, w.stft_res2_bn_bias, w.stft_res2_bn_mean, w.stft_res2_bn_var);
    
    % SE Attention
    y = se_attention(y, w.stft_se_fc1_weights, w.stft_se_fc2_weights);
    
    % Global Average Pooling
    feat_stft = global_avg_pool(y);  % [32, 1]
    
    % ==================== КЛАССИФИКАТОР ====================
    % Конкатенация
    features = [feat_dwt; feat_stft];  % [64, 1]
    
    % FC1
    z = linear(features, w.fc1_weights, w.fc1_bias);
    z = batchnorm1d(z, w.bn1_weight, w.bn1_bias, w.bn1_mean, w.bn1_var);
    z = relu(z);
    
    % FC2
    logits = linear(z, w.fc2_weights, w.fc2_bias);
    
    % Вероятности
    probabilities = sigmoid(logits);
    
    % Класс
    [~, idx] = max(probabilities);
    class_id = uint8(idx - 1);
end

% ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

function y = conv2d_dwt(x, weights, bias)
%#codegen
% 2D свертка для DWT: kernel 3x7, padding (1,3)
    coder.inline('never');
    
    [C_in, H_in, W_in] = size(x);
    [C_out, ~, KH, KW] = size(weights);
    
    H_out = cast(H_in + 2 - KH + 1, 'like', x);
    W_out = cast(W_in + 6 - KW + 1, 'like', x);
    
    % Padarray не поддерживается в codegen, делаем вручную
    pad_h = 1;
    pad_w = 3;
    x_pad = zeros(C_in, H_in + 2*pad_h, W_in + 2*pad_w, 'like', x);
    x_pad(:, pad_h+1:end-pad_h, pad_w+1:end-pad_w) = x;
    
    y = zeros(C_out, H_out, W_out, 'like', x);
    
    for oc = 1:C_out
        for oh = 1:H_out
            for ow = 1:W_out
                val = bias(oc);
                for ic = 1:C_in
                    for kh = 1:KH
                        for kw = 1:KW
                            val = val + x_pad(ic, oh+kh-1, ow+kw-1) * weights(oc, ic, kh, kw);
                        end
                    end
                end
                y(oc, oh, ow) = val;
            end
        end
    end
end

function y = conv2d_stft(x, weights, bias)
%#codegen
% 2D свертка для STFT: kernel 3x3, padding (1,1)
    coder.inline('never');
    
    [C_in, H_in, W_in] = size(x);
    [C_out, ~, KH, KW] = size(weights);
    
    H_out = cast(H_in + 2 - KH + 1, 'like', x);
    W_out = cast(W_in + 2 - KW + 1, 'like', x);
    
    % Ручной padding
    pad_h = 1;
    pad_w = 1;
    x_pad = zeros(C_in, H_in + 2*pad_h, W_in + 2*pad_w, 'like', x);
    x_pad(:, pad_h+1:end-pad_h, pad_w+1:end-pad_w) = x;
    
    y = zeros(C_out, H_out, W_out, 'like', x);
    
    for oc = 1:C_out
        for oh = 1:H_out
            for ow = 1:W_out
                val = bias(oc);
                for ic = 1:C_in
                    for kh = 1:KH
                        for kw = 1:KW
                            val = val + x_pad(ic, oh+kh-1, ow+kw-1) * weights(oc, ic, kh, kw);
                        end
                    end
                end
                y(oc, oh, ow) = val;
            end
        end
    end
end

function y = batchnorm2d(x, weight, bias, mean, var)
%#codegen
    coder.inline('never');
    
    eps = single(1e-5);
    [C, H, W] = size(x);
    y = zeros(C, H, W, 'like', x);
    
    for c = 1:C
        inv_std = single(1) / sqrt(var(c) + eps);
        scale = weight(c) * inv_std;
        offset = bias(c) - mean(c) * scale;
        for i = 1:H*W
            y(c, i) = x(c, i) * scale + offset;
        end
    end
end

function y = relu(x)
%#codegen
    y = max(x, single(0));
end

function y = residual_block(x, conv1_w, conv1_b, conv2_w, conv2_b, ...
                              bn1_w, bn1_b, bn1_m, bn1_v, ...
                              bn2_w, bn2_b, bn2_m, bn2_v)
%#codegen
    coder.inline('never');
    
    shortcut = x;
    
    y = conv2d_3x3(x, conv1_w, conv1_b);
    y = batchnorm2d(y, bn1_w, bn1_b, bn1_m, bn1_v);
    y = relu(y);
    
    y = conv2d_3x3(y, conv2_w, conv2_b);
    y = batchnorm2d(y, bn2_w, bn2_b, bn2_m, bn2_v);
    
    y = relu(y + shortcut);
end

function y = conv2d_3x3(x, weights, bias)
%#codegen
    coder.inline('never');
    
    [C_in, H_in, W_in] = size(x);
    [C_out, ~, KH, KW] = size(weights);
    
    H_out = cast(H_in + 2 - KH + 1, 'like', x);
    W_out = cast(W_in + 2 - KW + 1, 'like', x);
    
    % Ручной padding
    pad_h = 1;
    pad_w = 1;
    x_pad = zeros(C_in, H_in + 2*pad_h, W_in + 2*pad_w, 'like', x);
    x_pad(:, pad_h+1:end-pad_h, pad_w+1:end-pad_w) = x;
    
    y = zeros(C_out, H_out, W_out, 'like', x);
    
    for oc = 1:C_out
        for oh = 1:H_out
            for ow = 1:W_out
                val = bias(oc);
                for ic = 1:C_in
                    for kh = 1:KH
                        for kw = 1:KW
                            val = val + x_pad(ic, oh+kh-1, ow+kw-1) * weights(oc, ic, kh, kw);
                        end
                    end
                end
                y(oc, oh, ow) = val;
            end
        end
    end
end

function y = se_attention(x, fc1_w, fc2_w)
%#codegen
    coder.inline('never');
    
    [C, H, W] = size(x);
    reduction = 4;
    C_red = cast(C / reduction, 'like', x);
    
    % Global Average Pooling
    pooled = zeros(C, 1, 'like', x);
    for c = 1:C
        sum_val = single(0);
        for i = 1:H*W
            sum_val = sum_val + x(c, i);
        end
        pooled(c) = sum_val / single(H * W);
    end
    
    % FC1
    fc1_out = zeros(C_red, 1, 'like', x);
    for i = 1:C_red
        sum_val = single(0);
        for j = 1:C
            sum_val = sum_val + pooled(j) * fc1_w(j, i);
        end
        fc1_out(i) = relu(sum_val);
    end
    
    % FC2
    att = zeros(C, 1, 'like', x);
    for i = 1:C
        sum_val = single(0);
        for j = 1:C_red
            sum_val = sum_val + fc1_out(j) * fc2_w(i, j);
        end
        att(i) = single(1) / (single(1) + exp(-sum_val));
    end
    
    % Apply attention
    y = zeros(C, H, W, 'like', x);
    for c = 1:C
        for i = 1:H*W
            y(c, i) = x(c, i) * att(c);
        end
    end
end

function y = global_avg_pool(x)
%#codegen
    [C, H, W] = size(x);
    y = zeros(C, 1, 'like', x);
    for c = 1:C
        sum_val = single(0);
        for i = 1:H*W
            sum_val = sum_val + x(c, i);
        end
        y(c) = sum_val / single(H * W);
    end
end

function y = linear(x, weights, bias)
%#codegen
    N_in = size(weights, 2);
    N_out = size(weights, 1);
    y = zeros(N_out, 1, 'like', x);
    
    for i = 1:N_out
        val = bias(i);
        for j = 1:N_in
            val = val + x(j) * weights(i, j);
        end
        y(i) = val;
    end
end

function y = batchnorm1d(x, weight, bias, mean, var)
%#codegen
    eps = single(1e-5);
    N = length(x);
    y = zeros(N, 1, 'like', x);
    
    for i = 1:N
        inv_std = single(1) / sqrt(var(i) + eps);
        scale = weight(i) * inv_std;
        offset = bias(i) - mean(i) * scale;
        y(i) = x(i) * scale + offset;
    end
end

function y = sigmoid(x)
%#codegen
    y = single(1) ./ (single(1) + exp(-x));
end