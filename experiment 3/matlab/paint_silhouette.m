function [avg_silhouette_score, calinski_harabasz_score, dunnIndex] = paint_silhouette(featureData, clusters, centroids, title_name)
    % 绘制分类结果
    % Step 1:转化数据格式
    data = reshape(featureData, 2, length(featureData) / 2)';
    centroids = reshape(centroids, 2, length(centroids) / 2)';
    clusters = clusters';
    
    % Step 2: 计算轮廓系数
    silhouette_values = silhouette(data, clusters);
    avg_silhouette_score = mean(silhouette_values);
    
    % Step 3: 计算Calinski-Harabasz Index
    calinski_harabasz_score = evalclusters(data, clusters, 'CalinskiHarabasz').CriterionValues;

    % Step 4: 计算Dunn Index（需要自定义函数）
    dunnIndex = dunnsIndex(data, clusters);
    
    % 可视化结果
    figure;
    gscatter(data(:,1), data(:,2), clusters);
    hold on;
    plot(centroids(:,1), centroids(:,2), 'kx', 'MarkerSize', 15, 'LineWidth', 3);
    title(title_name);
    xlabel('Feature 1');
    ylabel('Feature 2');
    hold off;
    
    % 可视化轮廓系数
    figure;
    silhouette(data, clusters);
    title('轮廓系数图');
end