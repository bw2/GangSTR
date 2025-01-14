/*
Copyright (C) 2017 Melissa Gymrek <mgymrek@ucsd.edu>
and Nima Mousavi (mousavi@ucsd.edu)

This file is part of GangSTR.

GangSTR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GangSTR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GangSTR.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "src/options.h"

using namespace std;

Options::Options() {
  // // initialize options
  // reffa = "";
  // regionsfile = "";
  // outprefix = "";
  // dist_mean = 400;
  // dist_sdev = 50;
  // stutter_up = 0.01;
  // stutter_down = 0.02;
  // stutter_p = 0.95;
  // flanklen = 2000;
  // realignment_flanklen = 100;
  // frr_weight = 0.8;
  // enclosing_weight = 1.0;
  // spanning_weight = 1.0;
  // verbose = false;

  // CACNA1A_47
  rg_sample_string = "";
  reffa = "";
  regionsfile = "";
  outprefix = "";
  str_info_file = "";
  dist_mean.clear();
  dist_sdev.clear();
  grid_threshold = 10000;
  dist_distribution_size = 2000;
  coverage.clear();
  model_gc_cov = false;
  gc_bin_size = 0.1;
  gc_region_len = 1000;
  max_gc_regions = 100000;
  read_len = -1;
  min_score = 75;
  dist_man_set = false;
  stutter_up = 0.05;
  stutter_down = 0.05;
  stutter_p = 0.9;
  flanklen = 3000;
  realignment_flanklen = 100;
  regionsize = 2000;
  frr_weight = 1.0;
  enclosing_weight = 1.0;
  spanning_weight = 1.0;
  flanking_weight = 1.0;
  include_ggl = false;
  genome_wide = true;
  chrom = "";
  period.clear();
  verbose = false;
  very_verbose = false;
  quiet = false;
  ploidy = 2;
  num_boot_samp = 100;
  read_prob_mode = false;
  output_bootstrap = false;
  output_readinfo = false;
  //seed = time(NULL);
  // Fixed seed
  seed = 123;
  min_match = 5;
  use_cov = true;
  use_off = true;
  hist_mode = false;
  rescue_count = 0;
  min_reads_per_sample = 500;
  max_processed_reads_per_sample = 3000;
  skip_qscore = false;
}

Options::~Options() {}
