# WizardWave User Manual

    To begin, cd to the build folder root in your terminal. As you have received the project, it is already compiled. If you would like to recompile, do the following in the terminal after cding to there:
    rm -rf * (hit y)
    cmake .. (&& cmake --build . here to save time if you would like)
    cmake --build .

    After you have given it some time to compile, you should be able to find the AU file in build - WizardWave_artefacts - AU as WizardWave.component. If you are using a Mac, go to your Library folder in finder, then Audio, then Plug-Ins, Components. From here, click and drag that component file. It may ask you for your Mac's password, that's normal!
    
    From this step, after loading a DAW of choice (I like Live11!) you should be able to find WizardWave as an AU. When you open it, you should be greated with a purple panel with 5 dots, an ADSR toggle and an intensity meter. The ADSR toggle turns an ADSR on and off. By default, it is off meaning it should sustain forever. The intensity meter increases the strength of the processing effects on the instrument.
    
    To use the instrument is simple. Simply click 1 dot and then another in succession, this will create a line connecting the dots. If you have no connections, there will be no sound. If you start adding connections "externally" like you are drawing a pentagon, you will create basic waves
    1. external connection is sine
    2. external connections is triangle
    3 external connections is rect 10%
    4 external connections is rect 50%
    5 external connections is saw
    The connections "inside" of the pentagon, act as processing effects as follows.
    
    Top dot to bottom left - Reverb on
    Top right dot to top left - LPF on
    Top right dot to bottom left - HPF on
    Top left dot to bottom right - Distortion
    
    If at any point you get tired of the sound of a connection, simply right click on a line to get rid of it. The instrument plays on MIDI input! So make sure to write/play some beautiful MIDI and have fun!
