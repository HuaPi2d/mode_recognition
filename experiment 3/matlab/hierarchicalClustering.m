function clusters = hierarchicalClustering(data, num_clusters)
    % 绘制分类结果
    data = reshape(data, 2, length(data) / 2)';

    % Step 1: 计算距离矩阵
    distance_matrix = pdist(data);

    % Step 2: 进行分级聚类，使用组平均距离法
    Z = linkage(distance_matrix, 'average');

    % Step 3: 指定聚类类别数，例如3个聚类
    clusters = cluster(Z, 'maxclust', num_clusters);
    
    % 可视化结果
    figure;
    dendrogram(Z);
    title('Dendrogram of Hierarchical Clustering');

    figure;
    scatter(data(:,1), data(:,2), 50, clusters, 'filled');
    title('Clustered Data');
    xlabel('Feature 1');
    ylabel('Feature 2');
end