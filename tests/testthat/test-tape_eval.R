test_that("PPI ALR hardcoded estimate has low smo and smgrad values and constant Hessian and offset", {
  mnongamma <- ppi_egmodel(1)
  theta <- ppi_paramvec(beta = c(-0.95, -0.9, 0.5), AL = mnongamma$AL, bL = 0)
  set.seed(1234)
  Ycts <- rppi(1000, paramvec = theta)
  dsample <- round(Ycts * 100)/ 100
  dsample[, 3] <- 1 - rowSums(dsample[, 1:2])
  colMeans(dsample == 0)
  mean(apply(dsample, 1, min) == 0)  #0.96

  usertheta = ppi_paramvec(p = ncol(dsample),
                           bL = 0,
                           betap = tail(theta, 1))
  est_hardcoded <- ppi(dsample, paramvec = usertheta, trans = "alr", method = "hardcoded")

  hardcodedvals <- ppi_smvalues(dsample, paramvec = usertheta, evalparam = est_hardcoded$est$paramvec, trans = "alr")

  modelvals <- ppi_smvalues(dsample, paramvec = usertheta, evalparam = theta, trans = "alr")

  # for any given sample the estimate would be better than the true value, and can test this
  expect_lt(hardcodedvals$obj, modelvals$obj)
  expect_lt_v(abs(hardcodedvals$grad), abs(modelvals$grad))
  expect_lt_v(abs(hardcodedvals$grad), rep(1E-10, length(hardcodedvals$grad)))

  # expect the Hessian and offset to be the same - because they are independent of theta, only depend on the data
  expect_equal(hardcodedvals$hess, modelvals$hess)
  expect_equal(hardcodedvals$offset, modelvals$offset)
})


test_that("smobj() etc calculates the same as tape_eval()", {
  mnongamma <- ppi_egmodel(1)
  theta <- ppi_paramvec(beta = c(-0.95, -0.9, 0.5), AL = mnongamma$AL, bL = 0)
  set.seed(1234)
  Ycts <- rppi(1000, paramvec = theta)
  dsample <- round(Ycts * 100)/ 100
  dsample[, 3] <- 1 - rowSums(dsample[, 1:2])
  colMeans(dsample == 0)
  mean(apply(dsample, 1, min) == 0)  #0.96

  usertheta = ppi_paramvec(p = ncol(dsample),
                           bL = 0,
                           betap = tail(theta, 1))
  est_hardcoded <- ppi(dsample, paramvec = usertheta, trans = "alr", method = "hardcoded")


  vals_tape_eval <- ppi_smvalues(dsample, paramvec = usertheta, evalparam = est_hardcoded$est$paramvec, trans = "alr")
  vals_smobj <- ppi_cppad_values(dsample,
         stheta = est_hardcoded$est$paramvec,
         isfixed = t_u2i(usertheta),
         man = "Ralr", hsqfun = "ones", acut = 1)
  expect_equal(vals_tape_eval$obj, vals_smobj$obj)
  expect_equal(vals_tape_eval$grad, vals_smobj$grad)
  expect_equal(vals_tape_eval$hess, vals_smobj$hess, ignore_attr = TRUE)
})


