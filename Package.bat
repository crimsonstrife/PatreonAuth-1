rem Build the plugin
call RunUAT.bat BuildPlugin -Plugin="%CD%\PatreonAuth.uplugin" -Package="%CD%\Build\PatreonAuth" -Rocket

rem Zip the plugin
rd /s /q "./Build/PatreonAuth/Intermediate"
rd /s /q "./Build/PatreonAuth/Binaries"
7z a ./Build/PatreonAuth.zip ./Build/PatreonAuth/*