function [priority, w_cr] = AHP(cr, al)
    ri = [0.00 0.00 0.58 0.90 1.12 1.24 1.32 1.41 1.45 1.49]; % Theoretical random index
    n = length(cr);
    norm = cr./sum(cr); % Matrix normalized 
    w_cr = sum(norm') / n; % Weights
    cc = cr.*w_cr; % To calculate consistency
    wsv = sum(cc'); % Weighted sum value of consistency
    wc = wsv./w_cr; 
    lambda = sum(wc) / n;
    ci = (lambda - n) / (n - 1); % Consistency index
    cr = ci/ri(n); % Consistency Ratio
    priority = 0;
    if(length(al)~=1) 
        msg = "Criteria";
        for i=1:n
            [~, al_w(i,:)]= AHP(al{i}, i);
            fprintf(2,"Isolated priorities: ");
            disp(al_w(i,:));
        end
        priority = al_w'*w_cr';
    else 
        msg = compose("Alternatives (Cr:%d)",al);
    end
    if(cr < 0.10)
        fprintf(2, "[%s] Metrics reasonably consistent.\n", msg);
    else
        fprintf(2, "[%s] Judgements need reexamination (CR=%.2f).\n", msg, cr);
    end       
end
