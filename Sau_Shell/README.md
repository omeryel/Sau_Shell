SAU SHELL<br />
Sau Shell,Sakarya Üniversitesi öğrencileri tarafından Linux system üzerinde çalışan C programlama dili ile yazılan komut satırı istemcisidir.<br />
Komut Satırı Ne Yapar?<br />
Komut girildiğinde aşağıdakileri yapar:<br />
• Komut girildiğinde komut uzunluğu “NULL” değilse işleme sokar.<br />
• Komut satırındaki ögeleri kelimelere ayrıştırır.<br />
• Özel karakterleri(‘ | ’,’ < ’,’ > ’,vb.) kontrol eder.<br />
• Varsayılan olarak kabul edilen komutları(‘ cd ’,’ yardim ’,’ cik ’,’ sil ’) kontrol eder.<br />
• Eğer pipe(boru) işlemi varsa boru işlemini yürütür.<br />
• Sistem komutları ve kütüphaneler çocuk işlem oluşturularak ve execvp fonksiyonu çağrılarak koşturur.<br />
Komut Satırı Uygulaması<br />
• Komut satırının açıldığı dosya yolunu  “getcwd”  fonksiyonu ile ekrana basar.<br />
• Komut satırını kullanan o anki kullanıcı adını “getenv(‘USER’)” fonksiyonu ile ekrana basar.<br />
• Satırdaki ögeleri kelimelerine  “strsep(“”)” fonksiyonu kullanarak ayırır.Fonksiyon ögeleri boşluklara <br />balarak ayırır.Ayrıca sıfır uzunluktaki ögeleri(“NULL”) almaz.<br />
• Ögeleri ayırdıktan sonra ögeleri varsayılan komutlarla karşılaştırır. Karşılaştırma işlemini karakter <br />dizisiyle ögeleri “strcmp()” fonksiyonunu kullamıp karşılaştırarak  yapar.<br />
• “cd” komutu execvp fonksiyonu ile çalışmadığı için “cd” komutu varsayılan komutlara eklendi.”cd” komutunu <br />“chdir()” fonksiyonu ile yürütür.<br />
• Varsayılan komutlar haricinde komut girildiğinde system komutlarını yürütür.Sistem komutlarını yürütmek için<br /> çocuk işlem oluşturur ve onu koşturur ardında çocuk işlemin sonlanmasını bekler.<br />
• Pipe(boru)’ı “strsep(‘ | ’)” fonksiyonu ile tespit eder.Pipe’ı uygularken “|” işaretinden öncesi ve sonrasi <br />olmak üzere iki kısıma ayırır  ögeleri.Ardından her ikiden çocuk işlem oluşturur ve “execvp()” <br />fonksiyonu ile koşturur.Pipe kısaca , ilk öge koşturulduktan sonra ilk ögelerin çıktısının ikinci ögeye<br /> girdi olarak verelmesidir.<br />
I. Dosya tanımlayıcı için iki ögelik dizi tanımlanır.Dosya tanımlayıcısı 1 ise yazma , 0 ise okuma <br />tanımlanmaktadır.<br />
II. Pipe() pipe("boru”) işlemi gerçeklenir.<br />
III. İki tane çocuk işlem oluşturulur.<br />
IV. Birinci çocuk işleminde özetle şöyle yapılır:<br />
Burada işlem çıktısı pipe’a verilliyor.<br />
“stdout” a dosya tanımlayıcı 1 kopyalanıyor.<br />
Dosya tanımlayıcı 0 kapatılıyor.<br />
İlk komut “execvp” fonkiyonu ile koşturuluyor.<br />
V. İkinci çocuk işleminde özetle şöyle yapılır:<br />
Burada işleme girdi pipe’dan alınıyor.<br />
Dosya tanımlayıcı “stdin” e kopyalanıyor.<br />
Dosya tanımlayıcı 1 kapatılıyor.<br />
İkinci işlem “execvp ” fonksiyonu ile koşturuluyor.<br />
VI. Ve sonra ebeveyn işlem bekler iki çocuk işlemide sonlanması için<br />
Komut Satırını Çalıştırma<br />
-Makefile'in olduğu konumda Terminal(uç birim) açın<br />
-"make" yada "make do" yazın<br />
-Sonra "make run" yada "./shell" yazın<br />
-Olusturulan koşturulabilir programı silmek için "make clean" yazın<br />


