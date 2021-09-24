% My channel info (from api_keys tab in Thingspeak.com)
readChannelID = 1510482; 
rssiFieldId = 3;
locationFieldId = 4;
readAPIKey = 'E2VPDI6S4JOADSFY'; 
%'DateRange',[datetime('Sep 10, 2021 00:00'),datetime()] 
[data,timestamps,chInfo] = thingSpeakRead(readChannelID, 'ReadKey', readAPIKey, 'Fields',[3,2],'NumPoints',60);
sortedData = sortrows(data);
uc1 = unique( sortedData(:,1) ) ;
mc2 = accumarray( sortedData(:,1), sortedData(:,2), [], @mean ) ;
newData = [uc1, mc2(uc1)]

picture = imread('https://github.com/laudena/WiFiScanner/raw/main/sourceMap/floorPlan_onSmallSize_final.jpg');
[height,width,depth] = size(picture);

%The locations coordinates I used for measurements, on an image of the house. top-left corner is 0,0. (the location number# is the location on the array)
X =[377,477,615,735,794,673,412,533,700,445,491,540,761,683,400,533,631,718,786,808,663,744,614,422,858,315,292,315,229,280                ,295, 496, 400, 860, 818]'
Y =[1520,1518,1519,1580,1443,1443,1392,1434,1280,1241,1108,1167,1203,1069,920,1018,920,958,901,664,729,533,296,195,139,213,333,569,630,1019,1345,536, 740, 960, 1300]'
%Sample data for testing
%Signal = [-59,-86,-80.5,-83.5,-89.5,-87.5,-79.5,-75,-76,-69.275,-69,-59,-76,-58,-54,-51,-59,-61,-61,-50,-58,-71,-78,-70,-79,-76,-76,-66,-63,-66,-74,-70,-50,-92,-83]'

Signal = newData(:,2);
 length(X)
 length(Y)
 length(Signal)

% My original picture analysis was too long for thingspeak to handle, 
% so now I use the original coordinates i measured, with a 1/2 sized picture instead.
X = X/2
Y = Y/2

% Convert (-99)-(-50)dB to readable 0-100% 
strengthPercent = 2*(Signal+100)/100;

%calculate heat map
OverlayImage=[];
F = scatteredInterpolant(Y, X, strengthPercent,'linear');
for i = 1:height-1
   for j = 100:width
          OverlayImage(i,j) = F(i,j);
   end
end
alpha = (~isnan(OverlayImage))*0.6;
imshow(picture);
hold on
OverlayImage = imshow( OverlayImage );
caxis auto  
colormap( OverlayImage.Parent, jet );
colorbar( OverlayImage.Parent );
set( OverlayImage, 'AlphaData', alpha );
title('WiFi Signal Strength Heat-Map');
