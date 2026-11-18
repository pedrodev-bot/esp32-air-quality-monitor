# Base path where your version folders are located
$sourceBase = "C:\Users\Kabum\Downloads\ipe"

# Ordered list of versions: Folder name, Git Tag, and English Commit Message
$versions = @(
    @{ Folder = "Codigo v0.1.0"; Tag = "v0.1.0"; Msg = "feat: initial ESP32 firmware with MQ-5, DHT, non-blocking Wi-Fi, Telegram alerts and Wokwi setup" },
    @{ Folder = "Codigo v0.2.0"; Tag = "v0.2.0"; Msg = "feat: implement async loop with millis() and moving average filter for MQ-5" },
    @{ Folder = "Codigo v0.3.0"; Tag = "v0.3.0"; Msg = "feat: add OLED display manager for local sensor telemetry" },
    @{ Folder = "Codigo v0.3.1"; Tag = "v0.3.1"; Msg = "fix: stabilize OLED display rendering and sensor read intervals" },
    @{ Folder = "Codigo v0.3.2"; Tag = "v0.3.2"; Msg = "fix: initialize MQ-5 moving average buffer on first read to prevent false positives" },
    @{ Folder = "Codigo v0.4.0"; Tag = "v0.4.0"; Msg = "feat: add async MQTT manager for Home Assistant integration" },
    @{ Folder = "Codigo v0.5.0"; Tag = "v0.5.0"; Msg = "refactor: make Telegram alerts non-blocking with cooldown and consolidate MQTT loop" },
    @{ Folder = "Codigo v0.6.0"; Tag = "v0.6.0"; Msg = "feat: integrate OpenWeather API for regional AQI and cross-referenced Telegram alerts" },
    @{ Folder = "Codigo v0.7.0"; Tag = "v0.7.0"; Msg = "feat: migrate MQTT broker to Adafruit IO with authentication for dashboard PoC" }
)

foreach ($v in $versions) {
    $srcPath = Join-Path $sourceBase $v.Folder

    if (-Not (Test-Path $srcPath)) {
        Write-Host "Skipping $($v.Tag): folder not found at $srcPath" -ForegroundColor Yellow
        continue
    }

    # Remove old code files while keeping .git, .gitignore and this script
    Get-ChildItem -Exclude ".git", ".gitignore", "subir_versoes.ps1" | Remove-Item -Recurse -Force

    # Copy all files from the version folder into the repo
    Copy-Item -Path "$srcPath\*" -Destination "." -Recurse -Force
    Get-ChildItem -Recurse -Filter "secrets.h" | Remove-Item -Force

    # Stage, commit, and tag the version
    git add .
    git commit -m $v.Msg
    git tag $v.Tag

    Write-Host "Processed $($v.Tag) successfully!" -ForegroundColor Green
}

# Remove the helper script before pushing
Remove-Item "subir_versoes.ps1" -Force

# Push all commits and tags to GitHub at once
git push origin main --tags