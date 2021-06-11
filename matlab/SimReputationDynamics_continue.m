function [SF]=SimReputationDynamics_continue(b,c,N,q,e,ie,nIt);
% AssRule, Matrix that contains all assessment rules present in population
% ActRule, Matrix that contains all action rules present in the population
% e, perception error
% ie, implementation error
% q, Constant probability to observe a third-party interaction
% nIt, number of iterations of the indirect reciprocity game.
% N, Size of population
%% Setting up the objects
MC=ones(N,N); % Current image matrix; initially everyone is good
S=zeros(1,N); C=zeros(1,N); % S, Score for each round, C, Count for each round
%% Simulating the interactions
for t=1:nIt
%% Choosing a donor and a recipient and letting them interact
Do=randi(N); % Selecting a donor
Re=Do; while Re==Do, Re=randi(N); end % Selecting a different receiver
stD=MC(Do,Do); stR=MC(Do,Re); % Defining the players¡¯ standings
%% Payoff
prob=rand;
if prob<ie
RB=rand; 
Pi_Do=-c*RB; % Pay for scores applied with Random ActRule
Pi_Re=b*RB;  % Get for scores applied with Random ActRule
else
Pi_Do=-c*stR; % Pay for scores applied with Simple Standing
Pi_Re=b*stR;  % Get for scores applied with Simple Standing
end
%% Total Payoff Calculation
S(Do)=S(Do)+Pi_Do;
S(Re)=S(Re)+Pi_Re;
C(Do)=C(Do)+0.5;
C(Re)=C(Re)+0.5;
ActRule=stR; % Action Rule in Simple Standing
cp=ActRule;
%% Updating the donor¡¯s reputation
for Obs=1:N % Going through all all individuals as potential observers
if Obs==Do || Obs==Re || rand(1)<q % If individual observes interaction
stD=MC(Obs,Do); stR=MC(Obs,Re); % Retrieving the players¡¯ standings
prob=rand;
if prob<e % perception error
AssRule=rand;
elseif Obs<=N/2 % Half of the population is mutant.
AssRule=stR*(cp-1-0.02*stD*cp)+1; % L3 mutant (delta_1 = 0.02)
else
AssRule=stR*(cp-1)+1; % L3 resident
end
MC(Obs,Do)=AssRule;
end
end
end
%% Final Payoff
SF=S./C;
end