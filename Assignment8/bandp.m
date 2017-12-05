%bandpass filter
function y=bandp(x,f1,f3,fsl,fsh,rp,rs,Fs)

wp1=2*pi*f1/Fs;
wp3=2*pi*f3/Fs;
wsl=2*pi*fsl/Fs;
wsh=2*pi*fsh/Fs;
wp=[wp1 wp3];
ws=[wsl wsh];

[n,wn]=cheb1ord(ws/pi,wp/pi,rp,rs);
[bz1,az1]=cheby1(n,rp,wp/pi);

[h,w]=freqz(bz1,az1,256,Fs);
h=20*log10(abs(h));
%figure;plot(w,h);title('bandpass');grid on;
y=filter(bz1,az1,x);
end

