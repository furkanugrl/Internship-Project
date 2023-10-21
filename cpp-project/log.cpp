#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

using namespace std; 

// Circular logging için class tanımlaması
class CircularLogger {
public:
    // Logger'i maksimum dosya, dönüş aralığı ve günlük rehber ile başlatacak constructor
    CircularLogger(int maxFiles, int interval, const string& logDirectory)
        : maxFiles_(maxFiles), interval_(interval), logDirectory_(logDirectory) {
    }

    // Mesaj loglama
    void Log(const string& message) {
        RotateLogs(); // Log rotasyonunun kontrolü
        string timestamp = GetTimestamp(); // Anlık timestamp'i alma
        string logFile = logDirectory_ + timestamp + ".log"; // Log dosyasının ismini oluşturma
        
        // Dosyaya log mesajını yazma 
        ofstream logStream(logFile, ios::app);
        logStream << timestamp << " - " << message << endl;

        cout << timestamp << " - " << message << endl; // Mesajı console'de gösterme
    }

private:
    int maxFiles_; // Maximum log dosya sayısı
    int interval_; // Rotasyon aralığı (saniye)
    string logDirectory_; // Log dosyalarının bulunduğu alan
    vector<string> logFiles_; // Log dosyalarını takip etme

    // Timestamp'i elde etmek için fonksiyon
    string GetTimestamp() {
        auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        tm timeInfo = *gmtime(&now); // UTC zamanı
        stringstream timestamp;
        timestamp << put_time(&timeInfo, "%Y%m%d_%H%M%S"); // Timestampi düzenleme
        return timestamp.str();
    }

    // Log rotasyonunun kontrolü
    void RotateLogs() {
        if (logFiles_.size() >= maxFiles_) {
            remove(logFiles_.front().c_str()); // En eski log dosyasını silme
            logFiles_.erase(logFiles_.begin());
        }
        this_thread::sleep_for(chrono::seconds(interval_)); // Log rotasyon aralığını kontrol etme
    }
};

int main() {
    // 7 dosyalı, 86400 saniye aralıklı (1 gün) bir kaydedici ve "./logs/" konumunda logger oluşturma
    CircularLogger logger(7, 86400, "./logs/");
    
    // Test mesajlarını kaydetme
    for (int i = 0; i < 10; ++i) {
        logger.Log("Log entry " + to_string(i));
        this_thread::sleep_for(chrono::seconds(3600)); // Saatlik log simülasyonu
    }

    return 0;
}
