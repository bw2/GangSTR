# GangSTR
<img align="right" src="logo/GangSTR_logo_square.png" alt="GangSTR" width="200" height="200"> 

GangSTR is a tool for genome-wide profiling tandem repeats from short reads. A key advantage of GangSTR over existing genome-wide TR tools (e.g. [lobSTR](https://github.com/mgymrek/lobstr-code) or [hipSTR](https://github.com/tfwillems/HipSTR)) is that it can handle repeats that are longer than the read length.

GangSTR takes aligned reads (BAM) and a set of repeats in the reference genome as input and outputs a VCF file containing genotypes for each locus.

**Manuscript**: https://doi.org/10.1093/nar/gkz501

For questions on installation or usage, please open an issue, submit a pull request, or contact Nima Mousavi (mousavi@ucsd.edu).

For advanced topics such as those below, see the [GangSTR wiki](https://github.com/gymreklab/GangSTR/wiki).
* [Filtering GangSTR output using dumpSTR](https://github.com/gymreklab/GangSTR/wiki/Filtering-GangSTR-output)
* [Identifying repeat expansions](https://github.com/gymreklab/GangSTR/wiki/Identifying-repeat-expansions-using-GangSTR)
* [Creating custom reference panel](https://github.com/gymreklab/GangSTR/wiki/Creating-a-custom-reference-panel)


A Docker with GangSTR plus the dumpSTR filtering tool installed is available at [gymreklab/str-toolkit](https://hub.docker.com/r/gymreklab/str-toolkit) from Docker hub.

[Download](#download) | [Install](#install) | [Basic Usage](#usage) | [File formats](#formats) | [Reference files](#references)

<a name="download"></a>
## Download

The latest GangSTR release is available on the [releases page](https://github.com/gymreklab/GangSTR/releases).

For a list of TR references available, see [references](#references) below. 

<a name="install"></a>

## Basic Install

GangSTR requires third party packages [nlopt](https://nlopt.readthedocs.io/en/latest/), [gsl](https://www.gnu.org/software/gsl/doc/html/index.html), and [htslib](http://www.htslib.org//).

If you are installing from the tarball (which for most purposes you should be), the following instructions will install all dependencies as well as GangSTR itself. Both UNIX and Mac OSX are supported.

If you are attempting to compile and install directly from a cloned github repository (e.g. if you would like the latest and greatest unreleased feature or would like to contribute a fix or new feature), the following steps will not work and you should follow instructions under "Compiling from git source" below.

These steps have been tested and verfied against the following gcc compiler versions: 4.9.2, 5.4.0, 6.3.0, 7.3.0

If you are running as root:
```
tar -xzvf GangSTR-X.X.tar.gz
cd GangSTR-X.X
sudo ./install-gangstr.sh
```

If you are installing locally (e.g. on a cluster where you don't have root access):
```
tar -xzvf GangSTR-X.X.tar.gz
cd GangSTR-X.X
./install-gangstr.sh PREFIX
```


where `PREFIX` is a place you have write permissions. In most cases this will be your home directory, e.g. `$HOME`. If you install locally, make sure `$PREFIX/bin` is on your `PATH`.


Typing `GangSTR --help` should show a help message if GangSTR was successfully installed.

## Compiling from git source

To compile from git source, first make sure nlopt, gsl, and htslib are installed, then run the following:

```
# Clone the repo
git clone https://github.com/gymreklab/GangSTR
cd GangSTR/

# Generate the configure script
./reconf

# Compile and install GangSTR
./configure
make
make install
```

<a name="usage"></a>
## Basic usage
To run GangSTR using default parameters use the following command:
```
GangSTR --bam file.bam 
        --ref ref.fa 
        --regions regions.bed 
        --out outprefix 
```
Required parameters:
* **`--bam <file.bam,[file2.bam]>`** Comma separated list of input BAM files
* **`--ref`** Refererence genome (.fa)
* **`--regions`** Target TR loci (regions) (.bed)
* **`--out`** Output prefix

Additional general options:
* **`--targeted`** Run GangSTR in targeted mode. This mode should be used when targeting disease loci. (as opposed to genome-wide run)
* **`--chrom <string>`** Only genotype regions on this chromosome.
* **`--str-info <string>`** Tab file with additional per-STR info (e.g., expansion cutoff. see below for format)
* **`--period <string>`** Only genotype loci with periods (motif lengths) in this comma-separated list.
* **`--skip-qscore`** Skip calculation of Q-score (see **Q** field in VCF output).

Options for different sequencing settings
* **`--readlength <int>`** Preset read length (default: extract from alignments if not provided)
* **`--coverage <float>`** Preset average coverage, should be set for exome/targeted data. Comma separated list to specify for each BAM. (default: calculate if not provided)
* **`--model-gc-coverage`** Model coverage as a function of GC content. Requires genome-wide data. Experimental feature.
* **`--insertmean <float>`** Fragment length mean. (default: calculate if not provided)
* **`--insertsdev <float>`** Fragment length standard deviation. (default: calculate if not provided)
* **`--nonuniform`** Indicates non-uniform coverage in alignment file (i.e., used for exome sequencing). Using this flag removes the likelihood term corresponding to FRR count.
* **`--min-sample-reads <int>`** Minimum number of reads per sample.

Advanced parameters for likelihood model:
* **`--frrweight <float>`** Reset weight for FRR class in likelihood model. (default 1.0)
* **`--spanweight <float>`** Reset weight for Spanning class in likelihood model. (default 1.0)
* **`--enclweight <float>`** Reset weight for Enclosing class in likelihood model. (default 1.0)
* **`--flankweight <float>`** Reset weight for Flanking class in likelihood model. (default 1.0)
* **`--ploidy [1,2]`** Haploid (1) or diploid (2) genotyping. (default 2)
* **`--skipofftarget`** Skip off target regions included in the regions file.
* **`--readprobmode**` Only use read probabilities in likelihood model. (ignore class probability)
* **`--numbstrap <int>`** Number of bootstrap samples for calculating confidence intervals. (default 100)
* **`--grid-theshold <int>`** Use optimization rather than grid search to find MLE if search space (grid) contains more alleles than this threshold. Default: 10000
* **--rescue-count \<int\>** Number of regions that GangSTR attempts to rescue mates from (excluding off-target regions). Default: 0

Parameters for local realignment:
* **`--minscore <int>`** Minimun alignment score for accepting reads (default 75).
* **`--minmatch <int>`** Minimum matching basepairs required at the edge of the repeat region to accept flanking and enclosing reads (default 5).

Stutter model parameters:
* **`--stutterup <float>`** Stutter insertion probability (default 0.05)
* **`--stutterdown <float>`**	Stutter deletion probability (default: 0.05)
* **`--stutterprob <float>`**	Stutter step size parameter (default: 0.90)

Parameters for more detailed info about each locus:
* **`--output-readinfo`** Output a file containing extracted read information.
* **`--output-bootstraps`** Output a file containing bootstrap samples.
* **`--include-ggl`** Output GGL (special GL field) in VCF.

Additional optional parameters:
* **`-h,--help`** display help screen
* **`--quiet`** Don't print out anything
* **`--seed`** Random number generator initial seed
* **`-v,--verbose`** Print progress information (major steps)
* **`--very`** Print detailed progress information
* **`--version`** Print out the version of this software

<a name="formats"></a>
## File formats

GangSTR takes as input a BAM file of short read alignments, a reference set of TRs, and a reference genome, and outputs genotypes in a VCF file. Each of these formats is described below.

### BAM (`--bam`)
GangSTR requires a [BAM](https://samtools.github.io/hts-specs/SAMv1.pdf) file produced by an indel-sensitive aligner. The BAM file must be sorted and indexed e.g. by using `samtools sort` and `samtools index`. GangSTR currently only processes a single sample at a time.

### FASTA Reference genome (`--ref`)
You must input a reference genome in FASTA format. This must be the same reference build used to align the sequences in the BAM file.

### TR regions (`--regions`)
GangSTR requires a reference set of regions to genotype. This is a BED-like file with the following columns:

1. The name of the chromosome on which the STR is located
2. The start position of the STR on its chromosome
3. The end position of the STR on its chromosome
4. The motif length
5. The repeat motif

An optional 6th column may contain a comma-separated list of off-target regions for each TR. These are regions where misaligned reads for a given TR may be incorrectly mapped.

Below is an example file which contains 5 TR loci. Standard references for hg19 and GRCh38 can be obtained [below](#references).
**NOTE: The table header is for descriptive purposes. The BED file should not have a header**

| **CHROM** | **START** | **END** | **MOTIF_LEN** | **MOTIF** | **OFFTARGET (optional)** |
|-----------|-----------|---------|----------------|----------|------------|
|chr1	|10689	|10700|	5	|CGCGC|	|
| chr1  |  28589  | 28603  | 1 |      T    |   |
|chr4  |  11173|   11194  | 11   |   CGCCGGCGCGG |    |
|chr4   | 150889 | 150909 | 2    |   TG      ||
| chr19 | 45770205 | 45770264	| 3	| CAG	|chr2:163338502-163338506,chr3:197333949-197333955,chr6:16327632-16327646,chr6:170561926-170561931,chr7:122288209-122288215,chr8:133055822-133055827,chr11:28310883-28310888,chr17:4887671-4887677,chr18:55586148-55586165,chr19:13207866-13207871 |

#### --str-info
A tab delimited with the following header and format can be used to specify additional per locus information.
GangSTR currently supports expansion threshold through str-info. The threshold is specified in number of repeat copies, and it is used to calculate expansion probability. (See **QEXP** field in VCF format).
Note: The loci represented in this file are unique and duplicates should be removed. 

| **chrom** | **pos** | **end** | **thresh** |
| ------| ------| ------| ----- |
| chr1 | 26454 | 26465 | 50 | 
| chr1 | 31556 | 31570 | 20 | 
| chr1 | 35489 | 35504 | 25 | 

### VCF (output)
For more information on VCF file format, see the [VCF spec](http://samtools.github.io/hts-specs/VCFv4.2.pdf). In addition to standard VCF fields, GangSTR adds custom fields described below.

#### INFO fields

INFO fields contain aggregated statistics about each TR. The following custom fields are added:

| **FIELD** | **DESCRIPTION** |
|-----------|------------------|
| END | End position of the TR |
| PERIOD | Length of the repeat unit |
| GRID | Range of the optimization grid. Gives min and max repeat copy number considered |
| EXPTHRESH | The threshold copy number used to test for repeat expansions |
| STUTTERUP | The model probability to observe a stutter error increasing the repeat number |
| STUTTERDOWN | The model probability to observe a stutter error decreasing the repeat number |
| STUTTERP | The geometric parameter for modeling the stutter step size distribution| 
| RU | Repeat motif | 
| REF| Reference copy number (number of repeat units| 

#### FORMAT fields
FORMAT fields contain information specific to each genotype call. The following custom fields are added:

| **FIELD** | **DESCRIPTION** |
|-----------|------------------|
| GT | Genotype |
| DP | Read Depth (number of informative reads) |
| Q | Quality Score |
| REPCN | Genotype given in number of copies of the repeat motif |
| REPCI | 95% Confidence interval for each allele based on bootstrapping |
| RC | Number of reads in each class (enclosing, spanning, FRR, flanking) | 
| ML | Maximum likelihood | 
| INS| Insert size mean and stddev at the locus| 
| STDERR | Bootstrap standard error of each allele |
| QEXP | Prob. of no expansion, 1 expanded allele, both expanded alleles |
| GGL | Genotpye Likelihood of all pairs of alleles in the search space. Formatted similar to standard GL fields but with allele space defined by the INFO/GRID field |

**Q**: Quality score estimated alleles (REPCN), between 0 and 1. This quality score is a measure of GangSTR's confidence in short allele calls (shorter than read length). It gives the likelihood of the maximum likelihood genotype divided by the sum of likelihoods of all possible genotypes. This can be interpreted as a posterior probability with a uniform prior over all possible genotypes. Calculation of Q-score can be slow if the estimation search space (grid) is large. To skip this step, use `--skip-qscore` option.

**STDERR**: Standard error of estimated alleles using bootstrap method.

**QEXP**: Given estimated alleles, the likelihood plane, and an expansion threshold, this field shows three numbers: the probability of both alleles being smaller than the threshold, one allele larger and one smaller than threshold, and both alleles larger than threshold. The expansion threshold should be provided using `--str-info` field.

<a name="references"></a>
## GangSTR reference files

The following lists available references created using Tandem Repeats Finder. We update the reference periodically with additional loci or annotation changes. Note references must be unzipped before using with GangSTR. The file listed in bold is the current recommended version.

| **Reference build** | **Version** | **Link** | **Comment** |
| --------------------| ------------|----------|-------------|
| **hg19** | **ver13.1** | [hg19_ver13_1.bed.gz](https://s3.amazonaws.com/gangstr/hg19/genomewide/hg19_ver13_1.bed.gz) | Added two disease loci to the reference (C9ORF72 and FMR1) |
| **hg19** | **ver13** | [hg19_ver13.bed.gz](https://s3.amazonaws.com/gangstr/hg19/genomewide/hg19_ver13.bed.gz) | More strict removal of locus bundles |
| **hs37** | **ver13** | [hs37_ver13.bed.gz](https://s3.amazonaws.com/gangstr/hs37_ver13.bed.gz) | More strict removal of locus bundles |
| **hg38** | **ver13** | [hg38_ver13.bed.gz](https://s3.amazonaws.com/gangstr/hg38/genomewide/hg38_ver13.bed.gz) | More strict removal of locus bundles |
| hg19 | ver12 | [hg19_ver12.bed.gz](https://s3.amazonaws.com/gangstr/hg19/genomewide/hg19_ver12.bed.gz) | Motifs of up to 20 bps, includes ChrX and ChrY, Trimmed messy loci |
| hs37 | ver12 | [hs37_ver12.bed.gz](https://s3.amazonaws.com/gangstr/hs37_ver12.bed.gz) | Motifs of up to 20 bps, includes ChrX and ChrY, Trimmed messy loci |
| hg38 | ver12 | [hg38_ver12.bed.gz](https://s3.amazonaws.com/gangstr/hg38/genomewide/hg38_ver12.bed.gz) | Motifs of up to 20 bps, Trimmed messy loci |
| hg19 | ver8 | [hg19_ver8.bed.gz](https://s3.amazonaws.com/gangstr/hg19/genomewide/hg19_ver8.bed.gz) | Motifs of up to 15 bps |
| hg19 | ver10 | [hg19_ver10.sorted.bed.gz](https://s3.amazonaws.com/gangstr/hg19/genomewide/hg19_ver10.sorted.bed.gz) | Motifs of up to 20 bps, includes ChrX and ChrY |
| hs37 | ver8 | [hs37_ver8.bed.gz](https://s3.amazonaws.com/gangstr/hs37_ver8.bed.gz) | Motifs of up to 15 bps |
| hg38 | ver5 | [hg38_ver5.bed.gz](https://s3.amazonaws.com/gangstr/hg38/genomewide/hg38_ver5.bed.gz) | Motifs of up to 15 bps |
| hg38 | ver6 | [hg38_ver6.sorted.bed.gz](https://s3.amazonaws.com/gangstr/hg38/genomewide/hg38_ver6.sorted.bed.gz) | Motifs of up to 20 bps, includes ChrX and ChrY | 

The references below contain pre-defined off-target loci for target pathogenic loci (hg38 coordinates):

| **Locus** | **hg38 Link** | **hg19 Link** |
| ------| ------| ------|
| SCA1 | [SCA1_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/SCA1_hg38.bed) | [SCA1_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/SCA1_hg19.bed) |
| SCA2 | [SCA2_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/SCA2_hg38.bed) | [SCA2_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/SCA2_hg19.bed) |
| SCA3 | [SCA3_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/SCA3_hg38.bed) | [SCA3_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/SCA3_hg19.bed) |
| SCA6 | [SCA6_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/SCA6_hg38.bed) | [SCA6_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/SCA6_hg19.bed) |
| SCA7 | [SCA7_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/SCA7_hg38.bed) | [SCA7_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/SCA7_hg19.bed) |
| SCA8 | [SCA8_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/SCA8_hg38.bed) | [SCA8_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/SCA8_hg19.bed) |
| SCA12 | [SCA12_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/SCA12_hg38.bed) | [SCA12_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/SCA12_hg19.bed) |
| SCA17 | [SCA17_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/SCA17_hg38.bed) | [SCA17_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/SCA17_hg19.bed) |
| HTT | [HTT_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/HTT_hg38.bed) | [HTT_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/HTT_hg19.bed) |
| DM1 | [DM1_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/DM1_hg38.bed) | [DM1_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/DM1_hg19.bed) |
| FMR1 | [FMR1_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/FMR1_hg38.bed) | [FMR1_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/FMR1_hg19.bed) |
| C9ORF72 | [C9ORF72_hg38.bed](https://s3.amazonaws.com/gangstr/hg38/C9ORF72_hg38.bed) | [C9ORF72_hg19.bed](https://s3.amazonaws.com/gangstr/hg19/C9ORF72_hg19.bed) |

Non-human reference builds:

| **Reference build** | **Version** | **Link** | **Comment** |
| --------------------| ------------|----------|-------------|
| **mm10** | **ver2** | [mm10_ver2.bed.gz](https://s3.amazonaws.com/gangstr/mm10/mm10_ver2.bed.gz) |  |
| **mm9** | **ver2** | [mm9_ver2.bed.gz](https://s3.amazonaws.com/gangstr/mm9/mm9_ver2.bed.gz) |  |

## GangSTR callsets
GangSTR callsets on publicly available datasets.

| **Dataset** | **Reference build and version** | **Link** | 
| ----------- | -------------------- | ----------|
| NA12878, NA12891, NA12892 | hg19 v13.1 | [NA12878_trio_hg19_v13_1_filtered_level1.vcf.gz](https://s3.amazonaws.com/gangstr/callsets/NA12878_trio_hg19_v13_1_filtered_level1.vcf.gz) |

## Known Issues:
* Multi-sample run with `--bam-samps` may generate innacurate results. This issue is fixed in the repository and will be packaged in our next release.
* Multi-sample run with manually set insert size (using `--insertmean` and `--insertsdev`) may generate innacurate results. This issue is fixed in the repository and will be packaged in our next release.
