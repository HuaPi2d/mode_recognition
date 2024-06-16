function labelArray = dbscan_algorithm(featureData, epsilon, minPts)
    % Step 1:转化数据格式
    data = reshape(featureData, 2, length(featureData) / 2)';
    
    % Step 2:DBSCAN进行分类
    labelArray = dbscan(data, epsilon, minPts);
    
    % Step 3:可视化结果
    figure;
    gscatter(data(:,1), data(:,2), labelArray);
    title('DBSCAN Clustering');
    xlabel('X');
    ylabel('Y');
end