// Copyright (c) 2009-2017 The OTS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OTS_GLYF_H_
#define OTS_GLYF_H_

#include <new>
#include <utility>
#include <vector>

#include "ots.h"

namespace ots {
class OpenTypeMAXP;

class OpenTypeGLYF : public Table {
 public:
  explicit OpenTypeGLYF(Font *font, uint32_t tag)
      : Table(font, tag, tag), maxp(NULL), maxp_tracking({0,0,0}) { }

  bool Parse(const uint8_t *data, size_t length);
  bool Serialize(OTSStream *out);

 private:
  struct GidAtLevel {
    uint16_t gid;
    uint32_t level;
  };

  struct ComponentPointCount {
    uint32_t accumulated_component_points;
    std::vector<GidAtLevel> gid_stack;
  };

  bool ParseFlagsForSimpleGlyph(Buffer &glyph,
                                uint32_t num_flags,
                                uint32_t *flag_index,
                                uint32_t *coordinates_length);
  bool ParseSimpleGlyph(Buffer &glyph, int16_t num_contours);
  bool ParseCompositeGlyph(
      Buffer &glyph,
      ComponentPointCount* component_point_count);


  bool TraverseComponentsCountingPoints(
      Buffer& glyph,
      uint16_t base_glyph_id,
      uint32_t level,
      ComponentPointCount* component_point_count);

  OpenTypeMAXP* maxp;

  struct {
    uint16_t new_max_points;
    uint16_t new_max_c_points;
    uint16_t new_max_c_depth;
  } maxp_tracking;

  std::vector<std::pair<const uint8_t*, size_t> > iov;
};

}  // namespace ots

#endif  // OTS_GLYF_H_
