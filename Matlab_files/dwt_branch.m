function features = dwt_branch(x, w)
%#codegen
% DWT ветвь сети
% x: [8, 125, 120]
% features: [32, 1]

    % Conv2d: 8 -> 32, kernel 3x7, padding (1,3)
    x = conv2d_layer(x, w.dwt_conv_weight, w.dwt_conv_bias, 1, 3);
    x = batch_norm_layer(x, w.dwt_bn_weight, w.dwt_bn_bias, ...
                            w.dwt_bn_mean, w.dwt_bn_var);
    x = relu_layer(x);
    
    % Residual Block
    x = residual_block(x, ...
        w.dwt_res1_conv1_weight, w.dwt_res1_conv1_bias, ...
        w.dwt_res1_bn1_weight, w.dwt_res1_bn1_bias, ...
        w.dwt_res1_bn1_mean, w.dwt_res1_bn1_var, ...
        w.dwt_res1_conv2_weight, w.dwt_res1_conv2_bias, ...
        w.dwt_res1_bn2_weight, w.dwt_res1_bn2_bias, ...
        w.dwt_res1_bn2_mean, w.dwt_res1_bn2_var);
    
    % SE Attention
    x = se_attention(x, w.dwt_se_fc1_weight, w.dwt_se_fc1_bias, ...
                        w.dwt_se_fc2_weight, w.dwt_se_fc2_bias);
    
    % Adaptive Average Pooling to 1x1
    x = adaptive_avg_pool(x);
    
    % Flatten
    features = x(:);
end