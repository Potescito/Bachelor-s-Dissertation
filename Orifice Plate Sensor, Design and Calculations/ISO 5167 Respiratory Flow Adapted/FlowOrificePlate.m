%Function for calculating the Iterative Flow based on an Adaptation of ISO 5167-2
%---------------------------------------------------------
%In the following script, a function will be developed that implements the 
% calculation of the volumetric flow based on the ISO 5167 adapted standard.

function qv = FlowOrificePlate(rho, u, k, P1, P2, l1, l2, D, d)

    L1 = l1/D; %Pressure tap distance 1 normalized to diameter D
    L2 = l2/D; %Pressure tap distance 2 normalized to diameter D
    DeltaP = P1 - P2; %Differential pressure in Pa
    Beta = d/D; 

    %Constants New Expansion Coefficient
    a0 = 1;
    a1 = -0.5046;
    a2 = -0.1615;
    a3 = -0.05882;
    
    %Constants New Discharge Coefficient
    b1 = 0.59865;
    b2 = 0.81891;
    b3 = -0.86143;
    b4 = 0.25169;
    b5 = -2.2216;

    %New formulation for Discharge Coefficient
    C = b1 + b2*(Beta^3.75) + b3*(Beta^4)...
        + b4*(DeltaP/P1)^1.25 + b5*(Beta^4)*((DeltaP/P1)^2.25);

    %New formulation for Expansion Coefficient
    epsilon = a0 + a1*(DeltaP/P1) + a2*(DeltaP/P1)^2 + a3*(DeltaP/P1)^3;

    qm = C*epsilon*(pi/4)*(d^2)*(1-Beta^4)^(-0.5)*(2*DeltaP*rho)^(0.5); 
    qv = qm/rho; 

end