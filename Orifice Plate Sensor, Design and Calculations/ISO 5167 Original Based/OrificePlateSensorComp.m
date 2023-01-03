%Full Implementation of Orifice Board Sensor Based on ISO 5167
%---------------------------------------------------------
%The following script implements a complete calculation process for the 
% volumetric flow in an Orifice Plate sensor taking the differential 
% pressure from an Excel file. 

% Note1: Keep in mind that the equations are defined for procesing 
% positive differential pressure values, if you require a sign convention 
% to differentiate between inspiration and expration, it can be 
% implemented at the end of the script by multiplying by -1 the data that 
% correspond to one of them in the resulting vector

% Note2: In order to run this script correctly, the FowOrificePlate 
% and the excel files containing the differential pressure values must be 
% in the same folder

% Note3: The differential pressure meassured values must be in an excel xlsx file in 
% the first column 

clear all
close all

%Air Properties
rho = 1.225; %Air density in kg/m3
u = 1.849e-5; %Dynamic viscosity of air in Pa*s
k = 1.4; %Isentropic exponent of air 

%Orifice Plate Dimensions
prompt = {'Diameter (D)', 'Diameter (d)','Upstream pressure tap distance l1 (m)', 'Downstream pressure tap distance l2 (m)', 'Excel file name with the Differential Pressure data. Please include .xlsx'};
dlgtitle = 'Orifice Plate Sensor Dimensions';
dims = [1 50];
data = inputdlg(prompt,dlgtitle,dims);
D = str2double(data{1}); %%Diameter D in m (Ex: 0.014)
d = str2double(data{2}); %%Diameter d in m (Ex: 0.0065)
l1 = str2double(data{3}); %Upstream pressure tap distance l1 in m  (Ex: 0.0133)
l2 =str2double(data{4}); %Upstream pressure tap distance l2 in m  (Ex: 0.0133)
ExcNam = data{5}; %Excel name 

DATA = xlsread(ExcNam);
DeltaP = DATA(:,1);
P1 = 26281;
P2 = P1 - DeltaP;

for i=1:1:length(DeltaP)
    CalculatedFlow(i) = FlowOrificePlate(rho, u, k, P1, P2(i), l1, l2, D, d);
end

K=75;
CalculatedFlow = K.*CalculatedFlow;
figure (1)
hold on
grid on
plot(DeltaP, CalculatedFlow,'b--')
plot(DeltaP, DATA(:,2),'k')
title('Volumetric Modeled Flow and Mesured Flow', 'FontName', 'Times')
xlabel('Differential Pressure (Pa)','FontName', 'Times')
ylabel('Volumetric Flow (m^3/s)', 'FontName', 'Times')
legend({'Modeled flow','Measured flow'},'Location','southeast', 'FontName', 'Times')

RMSE_flow = sqrt(((sum(DATA(:,2) - CalculatedFlow(:,1)))^2)/length(CalculatedFlow));

