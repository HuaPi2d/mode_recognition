function [intra_class_scatter, inter_class_scatter] = calculate_intra_inter(data, labelArray)
    % 绘制分类结果
    features = reshape(data, 2, length(data) / 2)';
    labels = labelArray';
    data = [features, labels];

    % 初始化类内和类间离散度变量  
    intra_class_scatter = 0;  
    inter_class_scatter = 0;  
  
    % 创建一个空的cell数组来存储每个类别的数据  
    class_data = cell(max(labels), 1);  
  
    % 将数据分配到相应的类别中  
    for i = 1:size(data, 1)  
        class_idx = labels(i);  
        class_data{class_idx} = [class_data{class_idx}; features(i, :)];  
    end  
  
    % 计算类内离散度  
    for i = 1:length(class_data)  
        if ~isempty(class_data{i})  
            class_mean = mean(class_data{i}, 1);  
            distances = sqrt(sum((class_data{i} - class_mean).^2, 2));  
            intra_class_scatter = intra_class_scatter + sum(distances); % 或者取平均等其他聚合方式  
        end  
    end  
  
    % 计算全局均值  
    global_mean = mean(features, 1);  
  
    % 计算类间离散度  
    for i = 1:length(class_data)  
        if ~isempty(class_data{i})  
            class_mean = mean(class_data{i}, 1);  
            inter_class_distance = sqrt(sum((class_mean - global_mean).^2));  
            inter_class_scatter = inter_class_scatter + inter_class_distance^2; % 取平方和  
        end  
    end  
end