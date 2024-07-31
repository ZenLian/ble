#include "ble/dbus/PathUtils.hpp"

#include "test.hpp"

using namespace ble::PathUtils;

void test_ascendant()
{
    EXPECT_FALSE(IsAscendantOf("", ""));
    EXPECT_FALSE(IsAscendantOf("/", ""));
    EXPECT_FALSE(IsAscendantOf("", "/"));
    EXPECT_FALSE(IsAscendantOf("/", "/"));
    EXPECT_FALSE(IsAscendantOf("/org/bluez", "/org/bluez"));
    EXPECT_FALSE(IsAscendantOf("/org/bluez", "/"));
    EXPECT_FALSE(IsAscendantOf("/org/bluez", "/org"));

    EXPECT_TRUE(IsAscendantOf("/", "/org/bluez"));
    EXPECT_TRUE(IsAscendantOf("/org", "/org/bluez/hci0"));
    EXPECT_TRUE(IsAscendantOf("/org", "/org/bluez/hci0/dev_12_34_56_78_90_AB"));
    EXPECT_TRUE(IsAscendantOf("/org", "/org/org"));
}

void test_desendant()
{
    EXPECT_FALSE(IsDescendantOf("", ""));
    EXPECT_FALSE(IsDescendantOf("/", ""));
    EXPECT_FALSE(IsDescendantOf("", "/"));
    EXPECT_FALSE(IsDescendantOf("/", "/"));
    EXPECT_FALSE(IsDescendantOf("/org/bluez", "/org/bluez"));
    EXPECT_FALSE(IsDescendantOf("/org", "/org/bluez"));
    EXPECT_FALSE(IsDescendantOf("/", "/org/bluez"));

    EXPECT_TRUE(IsDescendantOf("/org/bluez", "/"));
    EXPECT_TRUE(IsDescendantOf("/org/bluez/hci0", "/"));
    EXPECT_TRUE(IsDescendantOf("/org/bluez/hci0/dev", "/org"));
    EXPECT_TRUE(IsDescendantOf("/org/org", "/org"));
}

void test_get_parent()
{
    EXPECT_TRUE(GetParent("").empty());
    EXPECT_TRUE(GetParent("/").empty());
    EXPECT_EQ(GetParent("/org"), "/");
    EXPECT_EQ(GetParent("/org/bluez"), "/org");
    EXPECT_EQ(GetParent("/org/bluez/hci0"), "/org/bluez");
}

void test_child_parent()
{
    EXPECT_TRUE(IsChildOf("/org", "/"));
    EXPECT_TRUE(IsParentOf("/", "/org"));
    EXPECT_TRUE(IsChildOf("/org/bluez", "/org"));
    EXPECT_TRUE(IsParentOf("/org", "/org/bluez"));
    EXPECT_TRUE(IsChildOf("/org/bluez/hci0", "/org/bluez"));
    EXPECT_TRUE(IsParentOf("/org/bluez", "/org/bluez/hci0"));

    EXPECT_FALSE(IsChildOf("", ""));
    EXPECT_FALSE(IsParentOf("", ""));
    EXPECT_FALSE(IsChildOf("/", "/"));
    EXPECT_FALSE(IsParentOf("/", "/"));
    EXPECT_FALSE(IsChildOf("", "/"));
    EXPECT_FALSE(IsParentOf("/", ""));
    EXPECT_FALSE(IsChildOf("/org/bluez", "/org/bluez"));
    EXPECT_FALSE(IsParentOf("/org/bluez", "/org/bluez"));
    EXPECT_FALSE(IsChildOf("/org", "/org/bluez"));
    EXPECT_FALSE(IsParentOf("/org/bluez", "/org"));
    EXPECT_FALSE(IsChildOf("/org/bluez/hci0", "/org"));
    EXPECT_FALSE(IsParentOf("/org", "/org/bluez/hci0"));
}

void test_split()
{
    std::vector<std::string> result;
    result = Split("");
    EXPECT_TRUE(result.empty());
    result = Split("/");
    EXPECT_TRUE(result.empty());

    result = Split("/org/bluez/hci0");
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "org");
    EXPECT_EQ(result[1], "bluez");
    EXPECT_EQ(result[2], "hci0");
}

void test_next_child()
{
    EXPECT_EQ(NextChild("/", "/org"), "/org");
    EXPECT_EQ(NextChild("/", "/org/bluez/hci0"), "/org");
    EXPECT_EQ(NextChild("/org", "/org/bluez/hci0"), "/org/bluez");
    EXPECT_EQ(NextChild("/org/bluez", "/org/bluez/hci0"), "/org/bluez/hci0");

    EXPECT_EQ(NextChild("/org", "/org"), "");
}

int main()
{
    test_split();
    test_ascendant();
    test_desendant();
    test_get_parent();
    test_child_parent();
    test_next_child();
}