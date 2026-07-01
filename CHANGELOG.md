# CHANGELOG
All notable changes to this **ESU** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/2.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
-   Hasil uji `hardware/REM-7.0`, REM Signal vs. Neutral Pad Resistance
-   `hardware/CTL-5.0`, menghapus blok Power Supply (Buck Converter 12V dan 5V) dan menambahkan blok INA-3.0
-   `hardware/PSU-1.0`, board Power Supply (Buck Converter 12V dan 5V) yang dipisahkan dari CTL-4.0
-   `hardware/PS-6.0`, penyesuaian letak edge connector karena perubahan board yang digunakan: CTL, PSU, MSD, dan REM.
-   Firmware `ESU_V1`: `feat/adaptive-REM-Lower-upper-limit`, fungsi `calculateREMLimit()` dengan metode Map-based Control untuk menentukan `REM_LOWER_LIMIT` dan `REM_UPPER_LIMIT` berdasarkan power setting pada output monopolar.
-   Firmware `ESU_V1`: `feat/adaptive-rmt-output`, fungsi `setDuration()` untuk rmt output dengan metode Map-based Control berdasarkan Power Setting yang terbaca oleh controller.

### Changed
-   Perbaikan code `readREM()` pada average samples dan fixed-point IIR filtering untuk stabilitas pembacaan REM saat trigger monopolar aktif di power tinggi (EMI effect)

### Fixed
-   Masalah noise yang ditimbulkan EMI yang mempengaruhi pembacaan REM saat trigger monopolar aktif

## [1.0.3] 2026-06-25
### Stack
-   Hardware
    - `PS-5.0`
    - `CTL-4.0`
    - `MSD-1.1`
    - `REM-7.0`
-   Firmware `ESU_V1`
-   UI

### Added
-   File PCB `hardware/REM-7.0`, reverse engineering dari REM board `TAKTVOLL v1.0 230927`
    - Versi sebelumnya terkendala pada stabilitas pembacaan saat trigger monopolar aktif di power tinggi (EMI effect).
    - Versi ini menerapkan komponen SMD pada sisi primer, desain double layer dengan top-bottom ground plane serta ruang soldering untuk can-style copper plate sebagai Faraday Cage.
-   File .pdf .dxf `hardware/REM-7.0`, untuk membuat trace pada copper board menggunakan laser.

## [1.0.1] 2026-01-01
### Added
-   File PCB `hardware/INA-3.0`, modul instrument amplifier untuk mengondisikan sinyal REM agar memiliki rentang pembacaan 0 - 5V.
-   Firmware controller `firmware/ESU_V1`

## [1.0.0] 2026-01-01
### Stack
-   Hardware
    - `PS-5.0`
    - `CTL-4.0`
    - `MSD-1.1`
    - `REM-4.0`
-   Firmware `ESU_V1`
-   UI

### Added
-   PCB Design file `hardware/PS-5.0`
-   PCB Design file `hardware/CTL-4.0`
-   PCB Design file `hardware/MSD-1.1`
-   PCB Design file `hardware/REM-4.0`
-   Firmware display `ui`
-   Firmware controller `firmware/ESP32-S3 PWM`
