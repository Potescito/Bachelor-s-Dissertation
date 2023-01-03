%Design of an Orifice Plate Sensor Based in an Adaptation of ISO 5167-2
%---------------------------------------------------------
% The following script develops the iterative methodology proposed by 
% ISO 5167 for the design of orifice plate sensor, by means of the
% calculation of the diameter d (Diameter of the Orifice in the plate)

clear all
close all

%Air Properties
rho = 1.225; %Air density in kg/m3
u = 1.849e-5; %Dynamic viscosity of air in Pa*s
k = 1.4; %Isentropic exponent of air 

%Defined Maximum Values of Flow, Pressure Taps and Diameter D
prompt = {'Maximum volumetric flow (m3/s)','Diameter (D)','Maximum upstream pressure tap (Pa)', 'Maximum downstream pressure tap (Pa)', 'Upstream pressure tap distance l1 (m)', 'Downstream pressure tap distance l2 (m)'};
dlgtitle = 'Defined Maximum Values of Flow, Distance and Pressure Taps and Diameter D';
dims = [1 50];
data = inputdlg(prompt,dlgtitle,dims);
qv = str2double(data{1}); %Max volumetric flow in m3/s (Ex: 0.014)
D = str2double(data{2}); %%Diameter D in m (Ex: 0.05)
P1 = str2double(data{3}); %Maximum pressure upstream tap in Pa (Ex: 26281)
P2 = str2double(data{4}); %Maximum pressure downstream tap in Pa (Ex: 25281)
l1 = str2double(data{5}); %Upstream pressure tap distance l1 in m  (Ex: 0.0254)
l2 =str2double(data{6}); %Upstream pressure tap distance l2 in m  (Ex: 0.0254)

qm = qv*rho; %Maximum mass flow in kg/s
DeltaP = P1-P2; %Differential pressure in Pa
L1 = l1/D; %Pressure tap distance 1 normalized to diameter D
L2 = l2/D; %Pressure tap distance 2 normalized to diameter D

ReD = (4*qm)/(pi*u*D); %Reynolds number

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

%-------Iterative Design Methodology by ISO 5167-------

A2 = (u*ReD)/(D*sqrt(2*DeltaP*rho)); %Invariant term

%First guess
epsilon = 0.97; %Expansion coefficient
C = 0.606; %Discharge coefficient

X1 = A2/(C*epsilon); %Iterative equation 1
d = D*((X1^2)/(1 + X1^2))^0.25;
Beta = d/D;
diff = 1;
while ~(abs(diff) < 1e-8)
    %New formulation for Discharge Coefficient
    C = b1 + b2*(Beta^3.75) + b3*(Beta^4)...
        + b4*(DeltaP/P1)^1.25 + b5*(Beta^4)*((DeltaP/P1)^2.25);
    %New formulation for Expansion Coefficient
    epsilon = a0 + a1*(DeltaP/P1) + a2*(DeltaP/P1)^2 + a3*(DeltaP/P1)^3;
    
    X1 = A2/(C*epsilon); %Iterative equation 1
    d = D*((X1^2)/(1 + X1^2))^0.25;
    X2 = (Beta^2)/(sqrt(1-Beta^4));%Iterative equation 2
    Beta = d/D;
    diff = (A2 - X2*C*epsilon)/A2;
end
fprintf('Diameter d = %f (m)', d)
msgbox(sprintf("The designed value for the diameter d is %f (m)", d))