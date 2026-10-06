#include <bits/stdc++.h>
using namespace std;

using Word = array<unsigned char, 8>;
const array<int, 16> permutation = {7, 0, 5, 6, 11, 15, 4, 3, 1, 13, 12, 14, 8, 9, 10, 2};
array<int, 16> inverse_permutation;
array<pair<int, int>, 256> inverse_pair;

int twice(int x) {
    return x < 8 ? x * 2 : (x - 8) * 2 ^ 3;
}

int rotate_right(int i) {
    return (i >> 1) | ((i & 1) << 2);
}

Word mix(const Word &input) {
    Word output{};
    for (int i = 0; i < 8; ++i) {
        const int j = rotate_right(i);
        output[i] = input[j] ^ twice(input[j ^ 1]);
    }
    return output;
}

Word unmix(const Word &input) {
    Word output{};
    for (int j = 0; j < 8; j += 2) {
        const int i0 = ((j << 1) & 7) | (j >> 2);
        const int i1 = (((j + 1) << 1) & 7) | ((j + 1) >> 2);
        const auto [x, y] = inverse_pair[input[i0] * 16 + input[i1]];
        output[j] = x;
        output[j + 1] = y;
    }
    return output;
}

Word encrypt(Word plaintext, const Word &key) {
    for (int round = 0; round < 6; ++round) {
        for (int i = 0; i < 8; ++i) plaintext[i] = permutation[plaintext[i] ^ key[i]];
        plaintext = mix(plaintext);
    }
    for (int i = 0; i < 8; ++i) plaintext[i] ^= key[i];
    return plaintext;
}

Word query(const Word &plaintext) {
    static const char hex[] = "0123456789abcdef";
    cout << "? ";
    for (int i = 7; i >= 0; --i) cout << hex[plaintext[i]];
    cout << endl;
    string response;
    if (!(cin >> response) || response == "-1") exit(0);
    if (response.size() != 8) exit(0);
    Word ciphertext{};
    for (int i = 0; i < 8; ++i) {
        const char c = response[7 - i];
        if (c >= '0' && c <= '9') ciphertext[i] = c - '0';
        else if (c >= 'a' && c <= 'f') ciphertext[i] = c - 'a' + 10;
        else exit(0);
    }
    return ciphertext;
}

uint32_t pack(const Word &key) {
    uint32_t value = 0;
    for (int i = 0; i < 8; ++i) value |= uint32_t(key[i]) << (4 * i);
    return value;
}

void answer(uint32_t key) {
    cout << "! " << key << endl;
    exit(0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    for (int i = 0; i < 16; ++i) inverse_permutation[permutation[i]] = i;
    array<bool, 256> seen{};
    for (int x = 0; x < 16; ++x) for (int y = 0; y < 16; ++y) {
        const int index = (x ^ twice(y)) * 16 + (y ^ twice(x));
        assert(!seen[index]);
        seen[index] = true;
        inverse_pair[index] = {x, y};
    }

    mt19937_64 random(uint64_t(chrono::steady_clock::now().time_since_epoch().count())
                      ^ (uint64_t(random_device{}()) << 32));
    array<uint16_t, 8> candidates;
    candidates.fill(0xffff);
    vector<pair<Word, Word>> observations;
    observations.reserve(4096);
    constexpr uint64_t enumeration_limit = 65536;

    for (int batch = 0; batch < 16; ++batch) {
        Word plaintext{};
        for (int i = 2; i < 8; ++i) plaintext[i] = random() & 15;
        array<array<int, 16>, 8> parity{};
        for (int x = 0; x < 16; ++x) for (int y = 0; y < 16; ++y) {
            plaintext[0] = x;
            plaintext[1] = y;
            const Word ciphertext = query(plaintext);
            observations.emplace_back(plaintext, ciphertext);
            const Word unmixed = unmix(ciphertext);
            for (int i = 0; i < 8; ++i) {
                for (int guess = 0; guess < 16; ++guess) {
                    if ((candidates[i] >> guess) & 1) {
                        parity[i][guess] ^= inverse_permutation[unmixed[i] ^ guess];
                    }
                }
            }
        }

        uint64_t combinations = 1;
        array<vector<int>, 8> choices;
        for (int i = 0; i < 8; ++i) {
            for (int guess = 0; guess < 16; ++guess) {
                if (parity[i][guess] != 0) candidates[i] &= uint16_t(~(1u << guess));
                if ((candidates[i] >> guess) & 1) choices[i].push_back(guess);
            }
            if (choices[i].empty()) return 0; // Invalid interaction.
            combinations *= choices[i].size();
        }
        if (combinations > enumeration_limit) continue;

        // The real key is in this complete product. Validate every candidate
        // with the actual cipher, rather than selecting a random nibble tuple.
        vector<uint32_t> verified;
        Word equivalent{};
        auto enumerate = [&](auto &&self, int position) -> void {
            if (position != 8) {
                for (int value : choices[position]) {
                    equivalent[position] = value;
                    self(self, position + 1);
                }
                return;
            }
            const Word key = mix(equivalent);
            for (const auto &[input, output] : observations) {
                if (encrypt(input, key) != output) return;
            }
            verified.push_back(pack(key));
        };
        enumerate(enumerate, 0);
        if (verified.size() == 1) answer(verified[0]);
        if (!verified.empty()) {
            array<uint16_t, 8> surviving{};
            for (uint32_t value : verified) {
                Word key{};
                for (int i = 0; i < 8; ++i) key[i] = (value >> (4 * i)) & 15;
                const Word moved_key = unmix(key);
                for (int i = 0; i < 8; ++i) surviving[i] |= uint16_t(1u << moved_key[i]);
            }
            candidates = surviving;
        }

        // The official integral attack is probabilistic as a recovery method.
        // After all 4096 queries, ambiguity is not mathematically excluded.
        // Any fallback key must at least reproduce every observed ciphertext.
        if (batch == 15 && !verified.empty()) answer(verified[0]);
    }
    // A candidate product still above the cap after 16 independent batches is
    // another possible (extremely unlikely in practice) recovery failure.
    return 0;
}
