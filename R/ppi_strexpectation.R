# Tests of the ppi() structure

expect_ppi_str <- function(object, p){
  act <- testthat::quasi_label(rlang::enquo(object), arg = "object")

  #check
  correctfirstnames <- isTRUE(setequal(names(object), c("est", "SE", "info")))
  if (!correctfirstnames){
    testthat::fail("Incorrect slot names")
  }

  correctest <- correcteststr(object$est, p)
  if ((length(object$SE) == 1) && (object$SE == "Not calculated.")){
    correctSE = list(SEnotcalculated = TRUE)
  } else {
    correctSE <- correcteststr(object$SE, p)
  }

  correctmethodstr <- isTRUE((class(object$info$method) == "character") & (nchar(object$info$method) > 0))
  passsummary <- list(
    slotnames = correctfirstnames,
    est_structure = correctest,
    SE_structure = correctSE,
    info_method_exists = correctmethodstr
  )
  if (all(unlist(passsummary))){
    testthat::succeed()
    return(invisible(act$val))
  }
  else {
    fails <- names(which(!unlist(passsummary)))
    fails <- gsub(".", "$", fails, fixed = TRUE)
    testthat::fail(
      sprintf("The following conditions fail: %s", paste(fails, collapse = ", ")))
  }
}

correcteststr <- function(est, p){
  correctestnames <- setequal(names(est), c("paramvec", "AL", "bL", "beta"))
  correctestdims <- list()
  if (correctestnames){
    correctestdims$paramvec <- (length(est$paramvec) == ppithetalength(p))
    correctestdims$AL <- isTRUE(all(dim(est$AL) == c(p-1, p-1)))
    correctestdims$bL <- (length(est$bL) == p-1)
    correctestdims$beta <- (length(est$beta) == p)
  }
  return(c(Names = correctestnames, correctestdims))
}
