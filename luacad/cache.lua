--
-- gestion de cache nef
--
-- tout passe par un md5
-- on peut meme avoir une cache sur disque ou internet
--
-- cache.init(true|false) intialize cache, use true to also use disk cache
-- cache.hash(slug) will return the hashing of a slug string. this hash is used for lookup
-- cache.lookup(h) will return a nef from memory or disk if it exists, or nil if not
-- cache.set(h,x) will set the mem (and disk) cache to nef x
--
-- cache.reset(h) -- cache.resetmem(h) -- cache.resetdisk(h) -> remove cache data
--
--


local cache={}

md5=md5 or require("md5")
nef=nef or require("nef")
lfs=lfs or require("lfs")

-- use hash to access
local storage={}
local diskcache=nil

local function diskfile(h)
    if diskcache then
        return diskcache.."/"..h
    end
    return nil
end



function cache.init(usememcache,usediskcache)
    print("cache initialized")
    if not usememcache then storage=nil end
    if usediskcache then
        diskcache=os.getenv("HOME").."/.luacad"
        if not lfs.attributes(diskcache) then
            print("cache created "..diskcache)
            lfs.mkdir(diskcache)
        end
        print("cache using disk cache "..diskcache)
    end
end

function cache.hash(slug)
    return md5.sumhexa(slug)
end

function cache.lookup(h)
    --print("lookup "..h)
    if storage then
        local k=storage[h]
        if k then
            --print("found "..h)
            return k
        end
    end
    --print("diskcache is "..tostring(diskcache))
    if diskcache then
        local k=diskfile(h)
        if lfs.attributes(k) then
            --print("loading "..h.." from disk")
            n=nef.load{name=k}
            if storage then storage[h]=n end
            return n
        end
    end
    return nil
end



function cache.resetmem(h)
    if storage then storage[h]=nil end
end
function cache.resetdisk(h)
    if diskcache then
        local k=diskfile(h)
        print("removing "..k)
        os.remove(k)
    end
end
function cache.reset(h)
    cache.resetmem(h)
    cache.resetdisk(h)
end

-- efface toute la memoire
function cache.flush()
    if storage then storage={} end
end


function cache.set(h,n)
    if storage then
        storage[h]=n
    end
    if diskcache then
        --print("saving to disk")
        n:save{name=(diskfile(h))}
    end
end

print("cache library loaded")

return cache

