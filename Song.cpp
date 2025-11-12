#include "Song.h"
#include <algorithm>
#include <cctype>
#include <ostream>

// 辅助函数实现
std::string trim_copy(const std::string &s) {
    if (s.empty()) return s;
    
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

std::string to_lower_copy(const std::string &s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string join_tags(const std::vector<std::string> &tags) {
    std::string result;
    for (size_t i = 0; i < tags.size(); ++i) {
        if (i > 0) result += ", ";
        result += tags[i];
    }
    return result;
}

// 静态成员初始化
int Song::next_id_ = 1;

// 构造函数
Song::Song(const std::string &title, const std::string &artist, 
           int duration_sec, int rating) 
    : id_(0), title_(""), artist_(""), duration_sec_(0), rating_(3), valid_(false) {
    
    std::string clean_title = trim_copy(title);
    std::string clean_artist = trim_copy(artist);
    
    if (clean_title.empty()) {
        return;
    }
    if (clean_artist.empty()) {
        return;
    }
    if (duration_sec <= 0) {
        return;
    }
    if (rating < 1 || rating > 5) {
        return;
    }
    
    id_ = next_id_++;
    title_ = clean_title;
    artist_ = clean_artist;
    duration_sec_ = duration_sec;
    rating_ = rating;
    valid_ = true;
}

// Setter 方法（只有这些需要实现）
bool Song::set_title(const std::string &t) {
    std::string clean_title = trim_copy(t);
    if (clean_title.empty()) {
        return false;
    }
    title_ = clean_title;
    return true;
}

bool Song::set_artist(const std::string &a) {
    std::string clean_artist = trim_copy(a);
    if (clean_artist.empty()) {
        return false;
    }
    artist_ = clean_artist;
    return true;
}

bool Song::set_duration(int sec) {
    if (sec <= 0) {
        return false;
    }
    duration_sec_ = sec;
    return true;
}

bool Song::set_rating(int r) {
    if (r < 1 || r > 5) {
        return false;
    }
    rating_ = r;
    return true;
}

// 标签管理
bool Song::add_tag(const std::string &tag) {
    std::string clean_tag = trim_copy(tag);
    if (clean_tag.empty()) {
        return false;
    }
    
    std::string lower_tag = to_lower_copy(clean_tag);
    for (const auto &existing_tag : tags_) {
        if (to_lower_copy(existing_tag) == lower_tag) {
            return false;
        }
    }
    
    tags_.push_back(clean_tag);
    return true;
}

bool Song::remove_tag(const std::string &tag) {
    std::string clean_tag = trim_copy(tag);
    std::string lower_tag = to_lower_copy(clean_tag);
    
    for (size_t i = 0; i < tags_.size(); ++i) {
        if (to_lower_copy(tags_[i]) == lower_tag) {
            tags_.erase(tags_.begin() + i);
            return true;
        }
    }
    
    return false;
}

// 关键词匹配
bool Song::matches_keyword(const std::string &kw) const {
    std::string clean_kw = trim_copy(kw);
    std::string lower_kw = to_lower_copy(clean_kw);
    
    if (lower_kw.empty()) return false;
    
    if (to_lower_copy(title_).find(lower_kw) != std::string::npos) {
        return true;
    }
    
    if (to_lower_copy(artist_).find(lower_kw) != std::string::npos) {
        return true;
    }
    
    for (const auto &tag : tags_) {
        if (to_lower_copy(tag).find(lower_kw) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

// 操作符重载
std::ostream& operator<<(std::ostream &os, const Song &s) {
    os << "[#" << s.id_ << "] " << s.artist_ << " - " << s.title_
       << " (" << s.duration_sec_ << "s) 评分" << std::string(s.rating_, '*');
    
    if (!s.tags_.empty()) {
        os << " [tags: " << join_tags(s.tags_) << "]";
    }
    
    return os;
}

bool operator<(const Song &a, const Song &b) {
    if (a.rating_ > b.rating_) return true;
    if (a.rating_ < b.rating_) return false;
    
    if (a.title_ < b.title_) return true;
    if (a.title_ > b.title_) return false;
    
    return a.id_ < b.id_;
}