// wfdb.hpp - minimal reader for PhysioNet WFDB records (MIT-BIH Arrhythmia Database)
//
// Supports what MIT-BIH needs:
//   .hea  header (text)
//   .dat  signal file, format 212 (and format 16)
//   .atr  annotation file, MIT binary annotation format

#pragma once
#include <string>
#include <vector>

namespace wfdb {

    //One line of the header per signal
    struct SignalInfo {

        std::string name; //e.g "100.dat"
        int format = 0; //storage format, 212 for MIT BIH
        double gain = 200.0; //ADC units per physical unit (per mV)
        int baseline = 0; //ADC value that means 0 mV
        std::string units = "mV"; //physical units
        int adcResolution = 0; //ADC resolution in bits
        int adcZero = 0; 
        int initialValue = 0; //first sample, used to sanity-check decoding
        int checkSum = 0; //16-bit sum of all samples
        bool has_initial_value = false;
        bool has_checksum = false;
        std::string description;  // lead name, e.g. "MLII"

    };

    struct Header {
        std::string record_name;
        int num_signals = 0;
        double sampling_frequency = 250.0;  // samples per second per signal
        long num_samples = 0;               // samples per signal (0 = unknown)
        std::vector<SignalInfo> signals;
        std::vector<std::string> comments;  // lines starting with '#'
    };

    struct Record {
        Header header;
        std::vector<std::vector<int>> adc;  // adc[signal][sample] = raw ADC value
        std::vector<std::string> warnings;  // checksum / length problems found while reading

        long length() const { return adc.empty() ? 0 : static_cast<long>(adc[0].size()); }
        bool is_valid(int signal, long i) const;        // false for "missing sample" markers
        double physical(int signal, long i) const;      // value in physical units (mV)
    };

    struct Annotation {
        long sample = 0;      // sample index the annotation points to
        int code = 0;         // numeric annotation code (1 = normal beat, 5 = PVC, ...)
        std::string symbol;   // one-character label, e.g. "N", "V", "+"
        int subtype = 0;
        int channel = 0;
        int num = 0;
        std::string aux;      // optional text, e.g. rhythm "(AFIB"
    };

    Header read_header(const std::string& hea_path);

    // Reads <dir>/<name>.hea and the .dat file(s) it refers to.
    Record read_record(const std::string& dir, const std::string& name);

    // Reads an annotation file such as <dir>/100.atr.
    std::vector<Annotation> read_annotations(const std::string& path);

    std::string code_to_symbol(int code);
    bool is_beat(const std::string& symbol);




} //namespace wfdb