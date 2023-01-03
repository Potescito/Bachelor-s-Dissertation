%Function for calculating the Iterative Flow based on ISO 5167-2
%---------------------------------------------------------
%In the following script, a function will be developed that implements the 
% iterative calculation of the volumetric flow based on the ISO 5167 
% standard.

function qv = FlowOrificePlate(rho, u, k, P1, P2, l1, l2, D, d)

    L1 = l1/D; %Pressure tap distance 1 normalized to diameter D
    L2 = l2/D; %Pressure tap distance 2 normalized to diameter D
    DeltaP = P1 - P2; %Differential pressure in Pa
    Beta = d/D; 

    %Expansion coefficient by ISO 5167
    epsilon = 1 - (0.351 + 0.256*Beta^4 + 0.93*Beta^8)*(1-(P2/P1)^(1/k));

    %-------Iterative Calculation Methodology by ISO 5167-------

    A1 = (epsilon*(d^2)*sqrt(2*DeltaP*rho))/(u*D*sqrt(1-Beta^4)); %Invariant term

    %First guess
    C = 0.1; %Discharge coefficient

    qm = (pi/4)*u*D*C*A1; %Iterative equation 1
    ReD = (4*qm)/(pi*u*D); %Iterative equation 2
    
    %Reader-Harris/Gallagher Equation for Discharge Coefficient
    M2 = (2*L2)/(1-Beta);
    A = (19000*Beta/ReD)^0.8;
    C = (0.5961) + (0.0261*Beta^2) - (0.216*Beta^8) ...
        + 0.000521*(((10^6*Beta)/ReD)^0.7) ...
        + (0.0188 + 0.0063*A)*(Beta^3.5)*(((10^6)/ReD)^0.3) ...
        + (0.043 + 0.080*exp(-10*L1) - 0.123*exp(-7*L1)) ...
        *(1-0.11*A)*((Beta^4)/(1-Beta^4))...
        - (0.031*(M2-0.8*(M2^1.1))*Beta^1.3)...
        + 0.011*(0.75-Beta)*(2.8-(D*1000)/25.4);
    
    qm = (pi/4)*u*D*C*A1;
    ReD = (4*qm)/(pi*u*D);

    %Reader-Harris/Gallagher Equation for Discharge Coefficient
    M2 = (2*L2)/(1-Beta);
    A = (19000*Beta/ReD)^0.8;
    C = (0.5961) + (0.0261*Beta^2) - (0.216*Beta^8) ...
        + 0.000521*(((10^6*Beta)/ReD)^0.7) ...
        + (0.0188 + 0.0063*A)*(Beta^3.5)*(((10^6)/ReD)^0.3) ...
        + (0.043 + 0.080*exp(-10*L1) - 0.123*exp(-7*L1)) ...
        *(1-0.11*A)*((Beta^4)/(1-Beta^4))...
        - (0.031*(M2-0.8*(M2^1.1))*Beta^1.3)...
        + 0.011*(0.75-Beta)*(2.8-(D*1000)/25.4);

    qm = (pi/4)*u*D*C*A1;
    qv = qm/rho;

end