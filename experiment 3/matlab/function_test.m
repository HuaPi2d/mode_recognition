a = false;
b = true;
% 假设数据存储在变量data中，其中最后一列是类别标签  
if a
    data = [
        -1.07 0.44;
        -0.90 0.60;
        -1.09 0.45;
        -1.65 0.33;
        -1.52 0.32;
        -1.50 0.32;
        -1.06 0.44;
        -0.90 0.60;
        -1.23 0.36;
        -1.52 0.32;
        -1.06 0.42;
        -1.23 0.36;
        -1.09 0.45;
        -1.17 0.34;
        -1.28 0.37;
    ];
    
    % Step 1: 设置聚类数
    num_clusters = 4;
    
    % Step 2: 使用K-means++进行聚类
    [clusters, centroids] = kmeans(data, num_clusters, 'Start', 'plus');
    
    disp(reshape(data', 1, 15*2));
    disp(centroids);
    disp(reshape(centroids', 1, 4*2));
    disp(clusters);
    
    result = paint_silhouette(reshape(data', 1, 15*2), clusters', reshape(centroids', 1, 4*2), "nm");
end

if a
    data = [
        -1.07 0.44;
        -0.90 0.60;
        -1.09 0.45;
        -1.65 0.33;
        -1.52 0.32;
        -1.50 0.32;
        -1.06 0.44;
        -0.90 0.60;
        -1.23 0.36;
        -1.52 0.32;
        -1.06 0.42;
        -1.23 0.36;
        -1.09 0.45;
        -1.17 0.34;
        -1.28 0.37;
    ];
    
    epsilon = 0.1; % 领域半径
    minPts = 3; % 最小点数
    
    labels = dbscan(data, epsilon, minPts);
    
    % 可视化结果
    figure;
    gscatter(data(:,1), data(:,2), labels);
    title('DBSCAN Clustering');
    xlabel('X');
    ylabel('Y');
end

if b
    data = [
        -1.07 0.44;
        -0.90 0.60;
        -1.09 0.45;
        -1.65 0.33;
        -1.52 0.32;
        -1.50 0.32;
        -1.06 0.44;
        -0.90 0.60;
        -1.23 0.36;
        -1.52 0.32;
        -1.06 0.42;
        -1.23 0.36;
        -1.09 0.45;
        -1.17 0.34;
        -1.28 0.37;
    ];
    dbscan_algorithm(reshape(data', 1, 15*2), 0.1, 3);
end