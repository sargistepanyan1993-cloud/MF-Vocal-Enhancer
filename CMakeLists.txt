cmake_minimum_required(VERSION 3.22)

project(MFProductionVocalEnhancer VERSION 1.0.0)

include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(MFProductionVocalEnhancer
juce_generate_juce_header(MFProductionVocalEnhancer)
    COMPANY_NAME "MF Production"
    BUNDLE_ID "com.mfproduction.vocalenhancer"
    PLUGIN_MANUFACTURER_CODE MFPr
    PLUGIN_CODE MfvE
    FORMATS VST3
    PRODUCT_NAME "MF Vocal Enhancer"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(MFProductionVocalEnhancer
    PRIVATE
        PluginProcessor.cpp
        PluginProcessor.h
        PluginEditor.cpp
        PluginEditor.h
)

target_compile_definitions(MFProductionVocalEnhancer
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)

target_link_libraries(MFProductionVocalEnhancer
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
