function logits = forward_pass(dwt_input, stft_input, weights)
%#codegen
% Прямой проход через мультимодальную сеть

    % STFT ветвь
    feat_stft = stft_branch(stft_input, weights);
    
    % DWT ветвь
    feat_dwt = dwt_branch(dwt_input, weights);
    
    % Конкатенация признаков
    features = [feat_dwt; feat_stft];  % [64, 1]
    
    % Классификатор
    logits = classifier_forward(features, weights);
end