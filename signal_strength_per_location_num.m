readChannelID = 1510482; 
rssiFieldId = 3;
locationFieldId = 4;
readAPIKey = 'E2VPDI6S4JOADSFY'; 
%'DateRange',[datetime('Sep 10, 2021 00:00'),datetime()] 
[data,timestamps,chInfo] = thingSpeakRead(readChannelID, 'ReadKey', readAPIKey, 'Fields',[3,2],'NumPoints',200);


sortedData = sortrows(data);
uc1 = unique( sortedData(:,1) ) ;
mc2 = accumarray( sortedData(:,1), sortedData(:,2), [], @mean ) ;
newData = [uc1, mc2(uc1)]
 
scatter (newData(:,1), newData(:,2))
xlabel('Location number'); 
ylabel('Signal Strength\newline RSSI [dB]'); 
title('RSSI averages in Location#');
