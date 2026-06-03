function features = stft_branch(x, w)
%#codegen
% STFT ветвь сети
% x: [8, 129, 21]
% features: [32, 1]

    % Conv2d: 8 -> 32, kernel 3x3, padding 1
    x = conv2d_layer(x, w.stft_conv_weight, w.stft_conv_bias, 1, 1);
    x = batch_norm_layer(x, w.stft_bn_weight, w.stft_bn_bias, ...
                            w.stft_bn_mean, w.stft_bn_var);
    x = relu_layer(x);
    
    % Residual Block
    x = residual_block(x, ...
        w.stft_res1_conv1_weight, w.stft_res1_conv1_bias, ...
        w.stft_res1_bn1_weight, w.stft_res1_bn1_bias, ...
        w.stft_res1_bn1_mean, w.stft_res1_bn1_var, ...
        w.stft_res1_conv2_weight, w.stft_res1_conv2_bias, ...
        w.stft_res1_bn2_weight, w.stft_res1_bn2_bias, ...
        w.stft_res1_bn2_mean, w.stft_res1_bn2_var);
    
    % SE Attention
    x = se_attention(x, w.stft_se_fc1_weight, w.stft_se_fc1_bias, ...
                        w.stft_se_fc2_weight, w.stft_se_fc2_bias);
    
    % Adaptive Average Pooling to 1x1
    x = adaptive_avg_pool(x);
    
    % Flatten
    features = x(:);
end