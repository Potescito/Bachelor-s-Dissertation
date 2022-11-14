% Comparison, theoretical computed weights
[eigenvectors, eigenvalues] = eig(cr);
lambda_num = max(max(real(eigenvalues)));
[~, pos_main_eig] = find(real(eigenvalues) == lambda_num);
norm_eig_w = eigenvectors(:, pos_main_eig) / sum(eigenvectors(:, pos_main_eig));
IRW = abs(w_cr' - norm_eig_w); % Comparison index btwn methods
