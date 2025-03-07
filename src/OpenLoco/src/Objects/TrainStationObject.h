#pragma once

#include "Engine/Limits.h"
#include "Map/Track/TrackEnum.h"
#include "Object.h"
#include "Types.hpp"
#include <OpenLoco/Core/EnumFlags.hpp>
#include <OpenLoco/Engine/World.hpp>
#include <array>
#include <cstddef>
#include <sfl/static_vector.hpp>
#include <span>

namespace OpenLoco
{
    namespace ObjectManager
    {
        struct DependentObjects;
    }
    namespace Gfx
    {
        class DrawingContext;
    }
    enum class TrainStationFlags : uint8_t
    {
        none = 0U,
        recolourable = 1U << 0,
        unk1 = 1U << 1, // Has no canopy??
    };
    OPENLOCO_ENABLE_ENUM_OPERATORS(TrainStationFlags);

#pragma pack(push, 1)
    struct TrainStationObject
    {
        static constexpr auto kObjectType = ObjectType::trainStation;

        using CargoOffset = std::array<World::Pos3, 2>;

        StringId name;
        uint8_t drawStyle;                         // 0x02
        uint8_t height;                            // 0x03 despite being uint8_t this is bigZ not smallZ
        World::Track::TrackTraitFlags trackPieces; // 0x04
        int16_t buildCostFactor;                   // 0x06
        int16_t sellCostFactor;                    // 0x08
        uint8_t costIndex;                         // 0x0A
        uint8_t var_0B;
        TrainStationFlags flags; // 0x0C
        uint8_t var_0D;
        uint32_t image; // 0x0E
        uint32_t var_12[4];
        uint8_t numCompatible; // 0x22
        uint8_t mods[7];
        uint16_t designedYear;                   // 0x2A
        uint16_t obsoleteYear;                   // 0x2C
        const std::byte* cargoOffsetBytes[4][4]; // 0x2E
        const std::byte* manualPower[16];

        void drawPreviewImage(Gfx::DrawingContext& drawingCtx, const int16_t x, const int16_t y) const;
        void drawDescription(Gfx::DrawingContext& drawingCtx, const int16_t x, const int16_t y, [[maybe_unused]] const int16_t width) const;
        bool validate() const;
        void load(const LoadedObjectHandle& handle, std::span<const std::byte> data, ObjectManager::DependentObjects*);
        void unload();
        sfl::static_vector<CargoOffset, Limits::kMaxStationCargoDensity> getCargoOffsets(const uint8_t rotation, const uint8_t nibble) const;
        constexpr bool hasFlags(TrainStationFlags flagsToTest) const
        {
            return (flags & flagsToTest) != TrainStationFlags::none;
        }
    };
#pragma pack(pop)
    static_assert(sizeof(TrainStationObject) == 0xAE);

    namespace TrainStation::ImageIds
    {
        constexpr uint32_t preview_image = 0;
        constexpr uint32_t preview_image_windows = 1;
        constexpr uint32_t totalPreviewImages = 2;
        // These are relative to var_12
        // var_12 is the imageIds per sequenceIndex (for start/middle/end of the platform)
        namespace Style0
        {
            constexpr uint32_t straightBackNE = 0;
            constexpr uint32_t straightFrontNE = 1;
            constexpr uint32_t straightCanopyNE = 2;
            constexpr uint32_t straightCanopyTranslucentNE = 3;
            constexpr uint32_t straightBackSE = 4;
            constexpr uint32_t straightFrontSE = 5;
            constexpr uint32_t straightCanopySE = 6;
            constexpr uint32_t straightCanopyTranslucentSE = 7;
            constexpr uint32_t diagonalNE0 = 8;
            constexpr uint32_t diagonalNE3 = 9;
            constexpr uint32_t diagonalNE1 = 10;
            constexpr uint32_t diagonalCanopyNE1 = 11;
            constexpr uint32_t diagonalCanopyTranslucentNE1 = 12;
            constexpr uint32_t diagonalSE1 = 13;
            constexpr uint32_t diagonalSE2 = 14;
            constexpr uint32_t diagonalSE3 = 15;
            constexpr uint32_t diagonalCanopyTranslucentSE3 = 16;
            constexpr uint32_t totalNumImages = 17;
        }
        namespace Style1
        {
            constexpr uint32_t totalNumImages = 8;
        }
    }
}
