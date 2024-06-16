function dunn_index = dunnsIndex(data, labels)
    % Dunn Index 计算
    clusters = unique(labels);
    num_clusters = length(clusters);
    
    % 计算类内最大距离
    intra_distances = zeros(num_clusters, 1);
    for i = 1:num_clusters
        cluster_points = data(labels == clusters(i), :);
        if size(cluster_points, 1) > 1
            intra_distances(i) = max(pdist(cluster_points));
        else
            intra_distances(i) = 0; % 单个点的类内距离为0
        end
    end
    max_intra_distance = max(intra_distances);
    
    % 计算类间最小距离
    inter_distances = inf(num_clusters, num_clusters);
    for i = 1:num_clusters
        for j = i+1:num_clusters
            cluster_points_i = data(labels == clusters(i), :);
            cluster_points_j = data(labels == clusters(j), :);
            distances = pdist2(cluster_points_i, cluster_points_j);
            inter_distances(i, j) = min(distances(:));
        end
    end
    min_inter_distance = min(inter_distances(inter_distances ~= inf));
    
    % 计算 Dunn Index
    dunn_index = min_inter_distance / max_intra_distance;
end