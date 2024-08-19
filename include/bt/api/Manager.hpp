#pragma once

#include "bt/types.hpp"

#include <memory>
#include <vector>
#include <functional>

namespace bt
{
/**
 * @class bt::Manager
 *
 * 对外 API 的顶层入口。
 */
class Manager
{
public:
    Manager();
    virtual ~Manager();

    /**
     * @brief 开始运行 Manager。
     *
     * 创建一个单独的线程以监听 DBus 事件，所有回调都在该线程中执行。
     */
    void Run();

    /**
     * @brief 返回任意一个可用的 Adapter。
     *
     * @return AdapterPtr
     */
    AdapterPtr GetDefaultAdapter() const;

    /**
     * @brief 返回所有 Adapters
     *
     * @return std::vector<AdapterPtr>
     */
    std::vector<AdapterPtr> GetAdapters() const;

    /**
     * @brief 根据 mac 地址返回 Adapter
     *
     * @param address Adapter 的地址（如 "12:34:56:78:90:AB"）
     * @return 无对应 Adapter 时返回 NULL
     */
    AdapterPtr GetAdapterByAddress(const std::string& address) const;

    /**
     * @brief 根据 DBus 路径返回 Adapter
     *
     * @param path DBus 路径（如 "/org/bluez/hci0"）
     * @return 无对应 Adapter 时返回 NULL
     */
    AdapterPtr GetAdapterByPath(const std::string& path) const;

    /** 事件回调 */
    std::function<void(AdapterPtr)> OnAdapterAdded;
    std::function<void(AdapterPtr)> OnAdapterRemoved;
    std::function<void(AdapterPtr)> OnAdapterChanged; // TODO: 需要其他参数吗？


private:
    std::unique_ptr<class ManagerPrivate> const _p;

    friend class ManagerPrivate;
};
} // namespace bt
