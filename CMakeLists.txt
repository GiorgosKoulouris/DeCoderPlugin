cmake_minimum_required(VERSION 3.15)

project(DeCoder VERSION 1.0.0)


set (BaseTargetName DeCoder)

add_subdirectory(JUCE)


juce_add_plugin("${BaseTargetName}"                             
    ICON_BIG Resources/Icon.png                     # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    ICON_SMALL Resources/Icon_Small.png
    COMPANY_NAME "QuantumAudio"
    COMPANY_WEBSITE www.gkoulouris.wixsite.com/home  
    COMPANY_EMAIL grgkoulouris@yahoo.gr             # Specify the name of the plugin's author
    IS_SYNTH FALSE                                  # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE                          # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                         # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                            # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE               # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD FALSE                   # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE QuAd                   # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE DeCd                                # A unique four-character plugin id with exactly one upper-case character
                                                    # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS Standalone VST3 AU                      # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "DeCoder")                         # The name of the final executable, which can differ from the target name

juce_generate_juce_header(${BaseTargetName})

target_sources(${BaseTargetName}
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/PluginEditor.h
        Source/PluginProcessor.h)

source_group(MySources FILES
    Source/PluginEditor.cpp
    Source/PluginProcessor.cpp
    Source/PluginEditor.h
    Source/PluginProcessor.h)

target_compile_definitions(${BaseTargetName}
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)


add_subdirectory(Resources)



target_link_libraries(DeCoder
    PRIVATE
        DeCoderData           
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
