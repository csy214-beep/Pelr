Welcome to the Pelr wiki!

**Program structure**

```mermaid
graph TD
    subgraph "Third Party APIs"
        xunfeiTTS[xunfei TTS]
        Ollama[Ollama - Local AI Model]
        OpenWeather[OpenWeather API]
    end

    subgraph "Local Storage"
        logFolder[Log Folder *.log]
        userFolder[User Folder *.dat]
        resources[Resources]
    end

    PelrTTS[Pelr TTS Server]
    Pelr[Pelr Core]

    subgraph "User Interface"
        bubbleBox[Bubble Box - Display Text]
        Live2D[Live2D Model]
        keyLabel[Key Monitor]
        window[Main Window]
    end

    %% TTS API connections
    Pelr -->|TTS Request| PelrTTS
    PelrTTS -->|API Call| xunfeiTTS
    xunfeiTTS -->|Audio Response| PelrTTS
    PelrTTS -->|Forward Audio| Pelr

    %% External API connections
    Pelr -->|AI Query| Ollama
    Pelr -->|Weather Data| OpenWeather

    %% Local storage connections
    Pelr -->|Write Logs| logFolder
    Pelr -->|Read/Write User Data| userFolder
    Pelr -->|Load Resources| resources

    %% UI connections
    Pelr -->|Display Text| bubbleBox
    Pelr -->|Monitor Input| keyLabel
    window -->|Configuration| Pelr
    Pelr -->|Control Model| Live2D

    bubbleBox -->|Trigger Playback| Pelr

    %% Styling
    classDef api fill:#e1f5fe,stroke:#01579b;
    classDef storage fill:#fff3e0,stroke:#e65100;
    classDef core fill:#f3e5f5,stroke:#4a148c;
    classDef ui fill:#e8f5e9,stroke:#1b5e20;

    class xunfeiTTS,Ollama,OpenWeather api;
    class logFolder,userFolder,resources storage;
    class PelrTTS,Pelr core;
    class bubbleBox,Live2D,keyLabel,window ui;
```


[IMPORTANT](https://gitee.com/Pfolg/Pelr/wikis/IMPORTANT)

[使用方法](https://gitee.com/Pfolg/Pelr/wikis/%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95)

技术栈

+ [C++ 工具链](https://gitee.com/Pfolg/Pelr/wikis/Cpp-%E5%B7%A5%E5%85%B7%E9%93%BE)

+ [Python 工具链](https://gitee.com/Pfolg/Pelr/wikis/Python-%E5%B7%A5%E5%85%B7%E9%93%BE)

[构建流程](https://gitee.com/Pfolg/Pelr/wikis/%E6%9E%84%E5%BB%BA%E6%B5%81%E7%A8%8B)

