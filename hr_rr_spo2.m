clf;
%use ir to find exact valley locations 

%insert raw ir data
ir = IR;
red = RED;

%remove DC signal
ir_length = length(ir);
ir_sum = sum(ir);
ir_mean =ir_sum/ir_length ;
ir = smooth(ir - ir_mean);

red_length = length(red);
red_sum = sum(red);
red_mean =red_sum/red_length ;
red = smooth(red - red_mean);

%use peak detction to find peak
ir = smooth(ir);
red = smooth(red);
%findpeaks(ir,1,'MinPeakDistance',25);
[ir_peak_high,ir_peak_locs] = findpeaks(ir,1,'MinPeakDistance',25);
[red_peak_high,red_peak_locs] = findpeaks(red,1,'MinPeakDistance',25);

%invert ir and smooth
%use peak detction to find valley
%figure;
ir_n = smooth(- ir);
%findpeaks(ir_n,1,'MinPeakDistance',25);
[ir_valley_high,ir_valley_locs] = findpeaks(ir_n,1,'MinPeakDistance',25);
ir_valley_high = -ir_valley_high;
ir_valley_locs_count = length(ir_valley_locs);

%initialize spo2_cal array
spo2_cal = zeros(ir_valley_locs_count-1, 1);
heartrate = zeros(ir_valley_locs_count-1, 1);

%calculate heart rate
%hr_ir = bandp(ir,1,2,2/3,2.5,0.1,30,50);% bandpass 1HZ-2HZ
%hr_ir = smooth(hr_ir);

%calculate respiratory rate
rr_ir = bandp(ir,1/6,1/3,0.01,2/3,0.1,30,50);% bandpass 1/6HZ-1/3HZ
rr_ir = smooth(rr_ir);
[rr_ir_high,rr_ir_locs] = findpeaks(rr_ir,1,'MinPeakDistance',150);
findpeaks(rr_ir,1,'MinPeakDistance',150);
rr_ir_locs_count = length(rr_ir_locs);
resp_rate = zeros(rr_ir_locs_count-1, 1);
for i=1:1:rr_ir_locs_count-1
    resp_rate(i) = round(60 *50 /(rr_ir_locs(i+1) - rr_ir_locs(i)));
end

%use an_ratio betwen AC component of Ir & Red and DC component of Ir & Red for SPO2 
%calculate heartrate
for i=1:1:ir_valley_locs_count-1
    %fitting a straight line
    k = (ir_valley_high(i+1)-ir_valley_high(i)) / (ir_valley_locs(i+1)-ir_valley_locs(i));
    b = ir_valley_high(i) - k*ir_valley_locs(i);
    
    %find AC compoent
    ir_ac = ir_peak_high(i+1) - (k*ir_peak_locs(i+1) + b);
    red_ac = red_peak_high(i+1) - (k*red_peak_locs(i+1) + b);
    
    %find DC component
    ir_dc = k * ir_peak_locs(i+1) + b + ir_mean;
    red_dc = k * red_peak_locs(i+1) + b + red_mean;
    
    %calculate ratio
    ratio = (red_ac * ir_dc) / (red_dc * ir_ac);
    
    %spo2 and heartrate
    spo2_cal(i) = round(-45.060*ratio*ratio/10000 + 30.354*ratio/100 + 94.845);
    heartrate(i) = round(60 * 50 / (ir_peak_locs(i+1) - ir_peak_locs(i)));
end
%figure;
%subplot(211);plot(spo2_cal);
%subplot(212);plot(heartrate);
%figure;
%subplot(211);plot_fft(IR,50,1);grid on;title('Raw IR');
%subplot(212);plot_fft(rr_ir,50,1);grid on;title('Respiratory Rate ir');
%subplot(211);plot(IR(1:300));title('IR');
%subplot(212);plot(ir(1:300));title('ir');
%findpeaks(ir,1,'MinPeakDistance',25);
%findpeaks(ir_n,1,'MinPeakDistance',25);title('Find Valleys');



