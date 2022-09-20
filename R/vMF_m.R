#' @title Mean direction and rotation for von Mises Fisher
#' @export
vMF_stdY <- function(Y, m = NULL, w = NULL){
  if(is.null(m)){
    m <- vMF_m(Y)
  }
  Rtrans <- Directional::rotation(m, c(1, rep(0, length(m) - 1)))
  out <- Y %*% t(Rtrans)
  return(out)
}

#' @describeIn vMF_stdY The mean direction of a sample, which is also the maximum likelihood estimate for the mean direction parameter `m` in the von Mises Fisher distribution.
#' @export
vMF_m <- function(Y, w = NULL){
  if (is.null(w)){m <- colMeans(Y)}
  else {m <- apply(Y, MARGIN = 2, weighted.mean, w)}
  m <- m/sqrt(sum(m^2))
  return(m)
}