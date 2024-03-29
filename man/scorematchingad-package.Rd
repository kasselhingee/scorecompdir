% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/scorematchingad-package.R
\docType{package}
\name{scorematchingad-package}
\alias{scorematchingad}
\alias{scorematchingad-package}
\title{scorematchingad: Score Matching Estimation by Automatic Differentiation}
\description{
Hyvärinen's score matching \url{http://jmlr.org/papers/v6/hyvarinen05a.html} is a useful estimation technique when the normalising constant for a probability distribution is difficult to compute. This package implements score matching estimators using automatic differentiation in the CppAD library \url{https://github.com/coin-or/CppAD} and is designed for quickly implementing more estimators. General Windham robustification \url{https://www.jstor.org/stable/2346159} is also available. Already in the package are estimators for directional distributions (Mardia, Kent and Laha (2016) \href{https://arxiv.org/abs/1604.08470}{arXiv:1604.08470}) and the flexible Polynomially-Tilted Pairwise Interaction (PPI) model for compositional data. The latter estimators perform well when there are zeros, even many zeros (Scealy and Wood, 2023 \doi{10.1080/01621459.2021.2016422}; Scealy, Hingee, Kent, and Wood, 2024, accepted article).
}
\details{
This package's main features are
\itemize{
\item A general capacity to implement score matching estimators that use algorithmic differentiation to avoid tedious manual algebra.
The package uses \code{CppAD} and \code{Eigen} to differentiate model densities and compute the score matching discrepancy function (see \code{\link{scorematchingtheory}}).
The score matching discrepancy is usually minimised by solving a quadratic equation, but a method for solving numerically (through \code{\link[optimx:Rcgmin]{optimx::Rcgmin()}}) is also included.
Currently new models, new manifolds or new transforms require small additions to the source code of this package; eventually it would be nice to enable these additions interactively within \code{R}.
\item Score matching estimators for the Polynomially-Tilted Pairwise Interaction (PPI) model \insertCite{scealy2023sc,scealy2024ro}{scorematchingad}. See function \code{\link[=ppi]{ppi()}}.
\item Score matching and hybrid score matching estimators for von Mises Fisher, Bingham and Fisher-Bingham directional distributions \insertCite{mardia2016sc}{scorematchingad}. See \code{\link[=vMF]{vMF()}}, \code{\link[=Bingham]{Bingham()}} and \code{\link[=FB]{FB()}}.
\item Implementation of a modification of Windham's robustifying method \insertCite{windham1995ro}{scorematchingad} for many exponential family distributions. See \code{\link[=Windham]{Windham()}}.
For some models the density approaches infinity at some locations, creating difficulties for the weights in Windham's original method \insertCite{scealy2024ro}{scorematchingad}.
\insertNoCite{*}{scorematchingad}
}
}
\section{Acknowledgements}{
Colleagues Andrew T. A. Wood and John T. Kent played important roles in developing the statistical ideas and theory for score matching estimation for the PPI model \insertCite{scealy2024ro}{scorematchingad}.

We developed this package on Ngunnawal and Ngambri Country. We thank the Country for its influence.
}

\references{
\insertAllCited{}
}
\author{
\strong{Maintainer}: Kassel Liam Hingee \email{kassel.hingee@anu.edu.au} (\href{https://orcid.org/0000-0001-9894-2407}{ORCID})

Authors:
\itemize{
  \item Janice Scealy (\href{https://orcid.org/0000-0002-9718-869X}{ORCID})
}

Other contributors:
\itemize{
  \item Bradley M. Bell [copyright holder]
}

}
