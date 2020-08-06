/*
 * Copyright (C) 2020 HERE Europe B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 * License-Filename: LICENSE
 */

#pragma once

#include <olp/core/client/CancellationContext.h>
#include <olp/core/client/HttpResponse.h>
#include <olp/core/geo/tiling/TileKey.h>
#include <olp/dataservice/read/Types.h>

namespace olp {
namespace dataservice {
namespace read {

class PrefetchJob {
 public:
  PrefetchJob(PrefetchTilesResponseCallback user_callback,
              PrefetchStatusCallback status_callback, uint32_t task_count);

  PrefetchJob(const PrefetchJob&) = delete;
  PrefetchJob(PrefetchJob&&) = delete;
  PrefetchJob& operator=(const PrefetchJob&) = delete;
  PrefetchJob& operator=(PrefetchJob&&) = delete;

  ~PrefetchJob();

  client::CancellationContext AddTask();

  void CompleteTask(geo::TileKey tile);

  void CompleteTask(geo::TileKey tile, const client::ApiError& error);

  void CompleteTask(geo::TileKey tile, client::NetworkStatistics statistics);

  void CompleteTask(geo::TileKey tile, const client::ApiError& error,
                    client::NetworkStatistics statistics);

  void CancelOperation();

 protected:
  void CompleteTask(std::shared_ptr<PrefetchTileResult> result,
                    client::NetworkStatistics statistics);

 private:
  PrefetchTilesResponseCallback user_callback_;
  PrefetchStatusCallback status_callback_;
  uint32_t task_count_;
  const uint32_t total_task_count_;
  bool canceled_;

  std::mutex mutex_;
  std::vector<client::CancellationContext> tasks_contexts_;
  PrefetchTilesResult prefetch_result_;

  client::NetworkStatistics accumulated_statistics_;
};

}  // namespace read
}  // namespace dataservice
}  // namespace olp
