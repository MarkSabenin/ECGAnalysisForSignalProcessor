function y = conv2d_layer(x, weights, bias, pad_h, pad_w)
%#codegen
% 2D свертка
% x: [C_in, H_in, W_in]
% weights: [C_out, C_in, KH, KW]
% bias: [C_out]
% y: [C_out, H_out, W_out]

    [C_in, H_in, W_in] = size(x);
    [C_out, ~, KH, KW] = size(weights);
    H_out = H_in + 2*pad_h - KH + 1;
    W_out = W_in + 2*pad_w - KW + 1;
    % Паддинг
    x_pad = padarray(x, [pad_h, pad_w], 0, 'both');
    y = zeros(C_out, H_out, W_out, 'like', x);
    for oc = 1:C_out
        for oh = 1:H_out
            for ow = 1:W_out
                sum_val = bias(oc);
                for ic = 1:C_in
                    for kh = 1:KH
                        for kw = 1:KW
                            ih = oh + kh - 1;
                            iw = ow + kw - 1;
                            sum_val = sum_val + x_pad(ic, ih, iw) * weights(oc, ic, kh, kw);
                        end
                    end
                end
                y(oc, oh, ow) = sum_val;
            end
        end
    end
end
function y = batch_norm_layer(x, weight, bias, mean, var)
%#codegen
% Batch Normalization для инференса
% x: [C, H, W]
% weight, bias, mean, var: [C]
% y: [C, H, W]
    eps = single(1e-5);
    [C, H, W] = size(x);
    y = zeros(C, H, W, 'like', x);    
    for c = 1:C
        inv_std = single(1) / sqrt(var(c) + eps);
        scaled_weight = weight(c) * inv_std;
        shifted_bias = bias(c) - mean(c) * scaled_weight;
        for i = 1:H*W
            y(c, i) = x(c, i) * scaled_weight + shifted_bias;
        end
    end
end
function y = relu_layer(x)
%#codegen
    y = max(x, 0);
end
function y = residual_block(x, conv1_w, conv1_b, bn1_w, bn1_b, bn1_m, bn1_v, ...
                               conv2_w, conv2_b, bn2_w, bn2_b, bn2_m, bn2_v)
%#codegen
% Residual block с сохранением размеров
    shortcut = x;
    % Conv1 + BN1 + ReLU
    y = conv2d_layer(x, conv1_w, conv1_b, 1, 1);
    y = batch_norm_layer(y, bn1_w, bn1_b, bn1_m, bn1_v);
    y = relu_layer(y);
    % Conv2 + BN2
    y = conv2d_layer(y, conv2_w, conv2_b, 1, 1);
    y = batch_norm_layer(y, bn2_w, bn2_b, bn2_m, bn2_v);
    % Add + ReLU
    y = relu_layer(y + shortcut);
end
function y = se_attention(x, fc1_w, fc1_b, fc2_w, fc2_b)
%#codegen
% SE Attention блок
    [C, H, W] = size(x);
    reduction = 4;
    C_red = C / reduction;
    % Global Average Pooling
    pooled = zeros(C, 1, 'like', x);
    for c = 1:C
        pooled(c) = sum(x(c, :), 'all') / (H * W);
    end
    % FC1 + ReLU
    fc1_out = zeros(C_red, 1, 'like', x);
    for i = 1:C_red
        sum_val = fc1_b(i);
        for j = 1:C
            sum_val = sum_val + pooled(j) * fc1_w(i, j);
        end
        fc1_out(i) = relu_layer(sum_val);
    end
    % FC2 + Sigmoid
    att = zeros(C, 1, 'like', x);
    for i = 1:C
        sum_val = fc2_b(i);
        for j = 1:C_red
            sum_val = sum_val + fc1_out(j) * fc2_w(i, j);
        end
        att(i) = 1 / (1 + exp(-sum_val));
    end
    % Apply attention
    y = zeros(C, H, W, 'like', x);
    for c = 1:C
        y(c, :, :) = x(c, :, :) * att(c);
    end
end
function y = adaptive_avg_pool(x)
%#codegen
% Adaptive Average Pooling to 1x1
% x: [C, H, W]
% y: [C, 1, 1]
    [C, H, W] = size(x);
    y = zeros(C, 1, 1, 'like', x);
    
    for c = 1:C
        y(c) = sum(x(c, :), 'all') / (H * W);
    end
end
function logits = classifier_forward(features, w)
%#codegen
% Классификатор
% features: [64, 1]
% logits: [12, 1]
    % Linear 64 -> 128
    x = linear_layer(features, w.cls_fc1_weight, w.cls_fc1_bias);
    x = batch_norm_1d(x, w.cls_bn1_weight, w.cls_bn1_bias, ...
                         w.cls_bn1_mean, w.cls_bn1_var);
    x = relu_layer(x);
    % Dropout пропускаем в инференсе
    % Linear 128 -> 12
    logits = linear_layer(x, w.cls_fc2_weight, w.cls_fc2_bias);
end

function y = linear_layer(x, weight, bias)
%#codegen
% Полносвязный слой
% x: [N_in, 1]
% weight: [N_out, N_in]
% bias: [N_out]
% y: [N_out, 1]
    N_in = size(weight, 2);
    N_out = size(weight, 1);
    y = zeros(N_out, 1, 'like', x);
    for i = 1:N_out
        sum_val = bias(i);
        for j = 1:N_in
            sum_val = sum_val + x(j) * weight(i, j);
        end
        y(i) = sum_val;
    end
end
% -------------------------------------------------------------------------
% batch_norm_1d.m
% -------------------------------------------------------------------------
function y = batch_norm_1d(x, weight, bias, mean, var)
%#codegen
% 1D Batch Normalization
    eps = single(1e-5);
    C = length(x);
    y = zeros(C, 1, 'like', x);
    
    for c = 1:C
        inv_std = 1 / sqrt(var(c) + eps);
        scaled_weight = weight(c) * inv_std;
        shifted_bias = bias(c) - mean(c) * scaled_weight;
        y(c) = x(c) * scaled_weight + shifted_bias;
    end
end