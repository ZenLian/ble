#pragma once

#include <string>
#include <vector>
#include <algorithm>

// NOTE: 假定所有函数的输入都是合法的 Object Path
namespace bt::PathUtils
{
    // "/org/bluez/hci0" -> {"org", "bluez", "hci0"}
    inline std::vector<std::string> Split(const std::string &path)
    {
        std::vector<std::string> result;

        if (path.empty() || path == "/")
        {
            return result;
        }

        auto iter = path.begin() + 1;
        auto end = path.end();
        while (iter != end)
        {
            auto next = std::find(iter, path.end(), '/');
            result.push_back(std::string(iter, next));
            if (next == end)
            {
                break;
            }
            iter = next + 1;
        }

        return result;
    }

    inline bool IsAscendantOf(const std::string &base, const std::string &path)
    {
        if (base.empty() || path.empty() || base == path)
            return false;

        if (base == "/")
            return true;

        return path.find(base) == 0;
    }

    inline bool IsDescendantOf(const std::string &path, const std::string &base)
    {
        return IsAscendantOf(base, path);
    }

    inline std::string GetParent(const std::string &path)
    {
        if (path.empty() || path == "/")
            return std::string();
        size_t pos = path.rfind("/");
        if (pos == 0)
            return "/";
        return path.substr(0, pos);
    }

    inline bool IsParentOf(const std::string &parent, const std::string &child)
    {
        if (parent.empty() || child.empty())
            return false;
        return GetParent(child) == parent;
    }

    inline bool IsChildOf(const std::string &child, const std::string &parent)
    {
        return IsParentOf(parent, child);
    }

    inline std::string NextChild(const std::string &base, const std::string &path)
    {
        std::string result;
        // "/org", "/org/bluez/hci0" -> "/org/bluez"
        if (!IsAscendantOf(base, path))
        {
            return result;
        }
        auto base_elements = Split(base);
        auto elements = Split(path);
        for (size_t i = 0; i < base_elements.size() + 1 && i < elements.size(); i++)
        {
            result += "/" + elements[i];
        }
        return result;
    }
}