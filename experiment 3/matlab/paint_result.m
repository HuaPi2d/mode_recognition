function paint_result(filename)
%UNTITLED2 此处显示有关此函数的摘要
  
% 使用dlmread函数读取数据，默认使用空格作为分隔符  
data = dlmread(filename);  

  
% 提取x, y, 和size（第三列）  
x = data(:, 1);  
y = data(:, 2);  
categories = data(:, 3);  
  
% 获取唯一的类别  
unique_categories = unique(categories);  
num_categories = numel(unique_categories);  
  
% 使用jet colormap为每种类别分配颜色  
% 首先，我们需要一个从1到num_categories的连续索引  
[~, ~, category_indices] = unique(categories, 'stable'); % 'stable'选项确保顺序与原始数据中的顺序一致  
% 然后，使用这些索引从jet colormap中获取颜色  
colors = jet(num_categories); % 获取num_categories个颜色的jet colormap  
scatter_colors = colors(category_indices, :); % 为每个数据点分配颜色  
  
% 绘制散点图  
scatter(x, y, [], scatter_colors, 'filled'); % 使用颜色向量绘制散点图  
  
% 设置轴标签和标题（可选）  
xlabel('X');  
ylabel('Y');  
title('clusterResult');  

end