# the geo json geometry object we got from geojson.io
# geo_json_geometry = {
#   "type": "Polygon",
#   "coordinates": [
#     [
#       [
#         -122.52227783203125,
#         40.660847697284815
#       ],
#       [
#         -122.52227783203125,
#         40.987154933797335
#       ],
#       [
#         -122.01690673828124,
#         40.987154933797335
#       ],
#       [
#         -122.01690673828124,
#         40.660847697284815
#       ],
#       [
#         -122.52227783203125,
#         40.660847697284815
#       ]
#     ]
#   ]
# }

# geo_json_geometry = {
#         "type": "Polygon",
#         "coordinates": [
#           [
#             [
#               -13.798828125,
#               29.22889003019423
#             ],
#             [
#               -13.798828125,
#               61.85614879566797
#             ],
#             [
#               47.8125,
#               61.85614879566797
#             ],
#             [
#               47.8125,
#               29.22889003019423
#             ],
#             [
#               -13.798828125,
#               29.22889003019423
#             ]
#           ]
#         ]
#       }

geo_json_geometry = {
    "type": "Polygon",
    "coordinates": [
        [
            [-120.1519, 37.8014],
            [-120.1519, 37.9152],
            [-120.0049, 37.9152],
            [-120.0049, 37.8014],
            [-120.1519, 37.8014]
        ]
    ]
  }



# filter for items the overlap with our chosen geometry
geometry_filter = {
  "type": "GeometryFilter",
  "field_name": "geometry",
  "config": geo_json_geometry
}

# filter images acquired in a certain date range
date_range_filter = {
  "type": "DateRangeFilter",
  "field_name": "acquired",
  "config": {
    "gte": "2013-05-03T00:00:00.000Z",
    "lte": "2014-05-11T00:00:00.000Z"
  }
}

# filter any images which are more than 50% clouds
cloud_cover_filter = {
  "type": "RangeFilter",
  "field_name": "cloud_cover",
  "config": {
    "lte": 0.5
  }
}

# create a filter that combines our geo and date filters
# could also use an "OrFilter"
redding_reservoir = {
  "type": "AndFilter",
  "config": [date_range_filter]
}
