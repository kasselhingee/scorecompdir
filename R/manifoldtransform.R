#' @title Build an object specifying the manifold-transform pair.
#' @description Generate an object used to specify a manifold and transformation to the manifold. This specific to each type of data.
#' @param name Name of the manifold-transform pair. See details.
#' @return An `Rcpp::XPtr` object with attribute `name`.
#' @details
#' Available pairs are
#'  + "sphere" for square-root transformation from the simplex to the positive orthant of the sphere
#'  + "simplex" for the simplex without any transformation.
#'  + "Ralr" for the additive log-ratio transformation from the simplex to Euclidean space, using the final component of vectors in the denominator of the ratio.
#'  + "Snative" for the sphere without any transformation
#' @export
manifoldtransform <- function(name){
  out <- pmanifold(name)
  attr(out, "name") <- name
  return(out)
}
