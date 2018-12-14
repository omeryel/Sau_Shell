SAU SHELL
Sau Shell,Sakarya Üniversitesi öğrencileri tarafından Linux system üzerinde çalışan C programlama dili ile yazılan komut satırı istemcisidir.
Komut Satırı Ne Yapar?
Komut girildiğinde aşağıdakileri yapar:
• Komut girildiğinde komut uzunluğu “NULL” değilse işleme sokar.
• Komut satırındaki ögeleri kelimelere ayrıştırır.
• Özel karakterleri(‘ | ’,’ < ’,’ > ’,vb.) kontrol eder.
• Varsayılan olarak kabul edilen komutları(‘ cd ’,’ yardim ’,’ cik ’,’ sil ’) kontrol eder.
• Eğer pipe(boru) işlemi varsa boru işlemini yürütür.
• Sistem komutları ve kütüphaneler çocuk işlem oluşturularak ve execvp fonksiyonu çağrılarak koşturur.
Komut Satırı Uygulaması
• Komut satırının açıldığı dosya yolunu  “getcwd”  fonksiyonu ile ekrana basar.
• Komut satırını kullanan o anki kullanıcı adını “getenv(‘USER’)” fonksiyonu ile ekrana basar.
• Satırdaki ögeleri kelimelerine  “strsep(“”)” fonksiyonu kullanarak ayırır.Fonksiyon ögeleri boşluklara balarak ayırır.Ayrıca sıfır uzunluktaki ögeleri(“NULL”) almaz.
• Ögeleri ayırdıktan sonra ögeleri varsayılan komutlarla karşılaştırır. Karşılaştırma işlemini karakter dizisiyle ögeleri “strcmp()” fonksiyonunu kullamıp karşılaştırarak  yapar.
• “cd” komutu execvp fonksiyonu ile çalışmadığı için “cd” komutu varsayılan komutlara eklendi.”cd” komutunu “chdir()” fonksiyonu ile yürütür.
• Varsayılan komutlar haricinde komut girildiğinde system komutlarını yürütür.Sistem komutlarını yürütmek için çocuk işlem oluşturur ve onu koşturur ardında çocuk işlemin sonlanmasını bekler.
• Pipe(boru)’ı “strsep(‘ | ’)” fonksiyonu ile tespit eder.Pipe’ı uygularken “|” işaretinden öncesi ve sonrasi olmak üzere iki kısıma ayırır  ögeleri.Ardından her ikiden çocuk işlem oluşturur ve “execvp()” fonksiyonu ile koşturur.Pipe kısaca , ilk öge koşturulduktan sonra ilk ögelerin çıktısının ikinci ögeye girdi olarak verelmesidir.
I. Dosya tanımlayıcı için iki ögelik dizi tanımlanır.Dosya tanımlayıcısı 1 ise yazma , 0 ise okuma tanımlanmaktadır.
II. Pipe() pipe("boru”) işlemi gerçeklenir.
III. İki tane çocuk işlem oluşturulur.
IV. Birinci çocuk işleminde özetle şöyle yapılır:
Burada işlem çıktısı pipe’a verilliyor.
“stdout” a dosya tanımlayıcı 1 kopyalanıyor.
Dosya tanımlayıcı 0 kapatılıyor.
İlk komut “execvp” fonkiyonu ile koşturuluyor.
V. İkinci çocuk işleminde özetle şöyle yapılır:
Burada işleme girdi pipe’dan alınıyor.
Dosya tanımlayıcı “stdin” e kopyalanıyor.
Dosya tanımlayıcı 1 kapatılıyor.
İkinci işlem “execvp ” fonksiyonu ile koşturuluyor.
VI. Ve sonra ebeveyn işlem bekler iki çocuk işlemide sonlanması için
Komut Satırını Çalıştırma
-Makefile'in olduğu konumda Terminal(uç birim) açın
-"make" yada "make do" yazın
-Sonra "make run" yada "./shell" yazın
-Olusturulan koşturulabilir programı silmek için "make clean" yazın


