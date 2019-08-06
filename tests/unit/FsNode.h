#pragma once

#include <map>
#include <memory>

#include <mega/types.h>
#include <mega/filefingerprint.h>

#include "DefaultedFileAccess.h"
#include "utils.h"

namespace mt {

// Represents a node on the filesystem (either file or directory)
class FsNode
{
public:
    FsNode(FsNode* parent, const mega::nodetype_t type, std::string name);

    MEGA_DISABLE_COPY_MOVE(FsNode)

    void setFsId(const mega::handle fsId)
    {
        mFsId = fsId;
    }

    mega::handle getFsId() const
    {
        return mFsId;
    }

    m_off_t getSize() const
    {
        return mSize;
    }

    mega::m_time_t getMTime() const
    {
        return mMTime;
    }

    const std::vector<mega::byte>& getContent() const
    {
        return mContent;
    }

    const mega::FileFingerprint& getFingerprint() const
    {
        return mFingerprint;
    }

    mega::nodetype_t getType() const
    {
        return mType;
    }

    const std::string& getName() const
    {
        return mName;
    }

    void setOpenable(const bool openable)
    {
        mOpenable = openable;
    }

    bool getOpenable() const
    {
        return mOpenable;
    }

    void setReadable(const bool readable)
    {
        mReadable = readable;
    }

    bool getReadable() const
    {
        return mReadable;
    }

    std::string getPath() const
    {
        std::string path = mName;
        auto parent = mParent;
        while (parent)
        {
            path = parent->mName + "/" + path;
            parent = parent->mParent;
        }
        return path;
    }

    const std::vector<const FsNode*>& getChildren() const
    {
        return mChildren;
    }

private:

    class FileAccess : public DefaultedFileAccess
    {
    public:
        explicit FileAccess(const FsNode& fsNode);

        bool fopen(std::string* path, bool, bool) override;

        bool frawread(mega::byte* buffer, unsigned size, m_off_t offset) override;

    private:
        const FsNode& mFsNode;
    };

    mega::handle mFsId = mega::UNDEF;
    m_off_t mSize = -1;
    mega::m_time_t mMTime = 0;
    std::vector<mega::byte> mContent;
    mega::FileFingerprint mFingerprint;
    std::unique_ptr<mega::FileAccess> mFileAccess = std::unique_ptr<mega::FileAccess>{new FileAccess{*this}};
    const FsNode* mParent = nullptr;
    const mega::nodetype_t mType = mega::TYPE_UNKNOWN;
    const std::string mName;
    bool mOpenable = true;
    bool mReadable = true;
    std::vector<const FsNode*> mChildren;
};

} // mt
