function generate_ecg_code()
    if ~exist('D:/trainedNetwork.mat', 'file')
        error('File D:/trainedNetwork.mat not found!');
    end
    cfg = coder.config('lib', 'ecoder', true);
    cfg.TargetLang = 'C';
    cfg.TargetLangStandard = 'C99 (ISO)';
    cfg.EnableDynamicMemoryAllocation = false;
    cfg.EnableVariableSizing = false;
    cfg.HardwareImplementation.ProdHWDeviceType = 'Analog Devices->Blackfin';
    cfg.RuntimeChecks = false;
    cfg.EnableVariableSizing = false;

    % DWT вход: [8, 125, 120] 
    dwt_type = coder.typeof(single(0), [8, 125, 120]);
    % STFT вход: [8, 129, 21]
    stft_type = coder.typeof(single(0), [8, 129, 21]);
    
    % Генерация кода
    fprintf('Generating code for ecg_predict...\n');
    
    codegen ecg_predict -args {dwt_type, stft_type} ...
                        -config cfg ...
                        -o ecg_predict_lib ...
                        -report
    output_dir = 'D:/generated_ecg_code';
    if ~exist(output_dir, 'dir')
        mkdir(output_dir);
    end
    
    copyfile('codegen/lib/ecg_predict/*.h', output_dir);
    copyfile('codegen/lib/ecg_predict/*.c', output_dir);
    
    fprintf('\nCopied to: %s\n', output_dir);
end