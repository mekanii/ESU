2025-02-07
  https://yopiediy.xyz/esp32-mcpwm-as-spwm-generator/
  Perancangan hardware PWM generator:
  • coding untuk generate PWM 10MHz that simulates a sine wave 500kHz menggunakan metode lookup table, timer interrupt, dan LEDC timer [Need Review]
  • Implementasi scaling SPWM calculation formula ke code menggunakan controller BPI-Leaf-S3 [Need Review]
  • Menulis dokumentasi design-controller: SPWM generator code [Need Review]
  • Uji fungsi scaling SPWM menggunakan oscilloscope [To Do]

  Alternatif PWM generator menggunakan MCPWM sebagai PWM generator
  • Mathematical Modeling dan implementation code

2025-02-06
  Perancangan hardware PWM generator:
  • coding untuk generate PWM 10MHz that simulates a sine wave 500kHz [In Progress]
  • Membuat kalkulasi scaling SPWM untuk varying amplitude dari 1% sampai 100% berdasarkan hasil ukur grafik SPWM amplitude 100% [Done]
  • Analisa perbandingan hasil hitung terhadap hasil ukur grafik [Done]
  • Menulis dokumentasi design-controller: grafik, datatable, dan analisa hitungan scaling SPWM calculation formula [Bone]
  • Implementasi scaling SPWM calculation formula ke code [To Do]

2025-02-05
  Perancangan hardware PWM generator:
  • coding untuk generate PWM 10MHz that simulates a sine wave 500kHz [In Progress]
  • Buat grafik SPWM 10MHz untuk amplitude 100%, 50%, 25%, 10%, 2% sebagai acuan [Done]
  • Mengukur Duty Cycle dari grafik SPWM [Done]

2025-02-04
  Rencana alternatif pengukuran daya output ESU:
  • Referensi produk Current Transformer high frequency [In Progress]
  • Referensi pengukuran Current Transformer high frequency [Done]

  Perancangan hardware PWM generator:
  • coding untuk generate PWM 10MHz that simulates a sine wave 500kHz [In Progress]

2025-02-03
  Rencana alternatif pengukuran daya output ESU:
  • Referensi produk osiloskop digital: Hantek, Rigol [Done]
  • Referensi produk Current Transformer high frequency [In Progress]
  • Referensi pengukuran Current Transformer high frequency [To Do]

2025-01-31
  Menulis document testing ESU:
  Output Power Stage Measurement Report
  • Menyusun gambar untuk komparasi [Done]
  • Hitung frekuensi modulasi [Need Review]
  • Hitung tegangan maksimum [In Progress]

2025-01-24
  Tes rangkaian mainboard ESU:
  • Setup generator output test connection - Bipolar & Monopolar [Done]
  • Rekam sinyal output - Bipolar standard [Done]
  • Rekam sinyal output - Monopolar cut (pure, blend1, blend2) [Done]
  • Rekam sinyal output - Monopolar coag (spray, forced) [Done]

2025-01-23
  Tes rangkaian mainboard ESU:
  • Setup oscillator test coonection
  • Rekam sinyal input MOSFET Driver / Output Controller dengan berbagai mode cut dan coag [Need Review]
  • Setup generator output test connection - Bipolar [In Progress]