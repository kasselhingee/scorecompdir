skip_on_cran()

test_that("prodsq weights match estimator2", {
  acut = 0.1
  p = 3

  #simulate
  beta = c(-0.3, -0.1, 3)
  n = 10
  set.seed(134)
  utabl <- MCMCpack::rdirichlet(n, beta+1)

  tapes <- buildsmotape("sphere", "dirichlet",
               rep(1/p, p), rep(NA, p),
               "prodsq", acut = acut)

  out <- cppad_closed(tapes$smotape, Y = utabl)

  hardcodedestimate <- dir_sqrt_prodh(utabl, acut)
  expect_equal(out$est, hardcodedestimate, ignore_attr = TRUE)
})



test_that("minsq weights match estimator2", {
  acut = 0.1
  p = 3
  tapes <- buildsmotape("sphere", "dirichlet",
                        rep(1/p, p), rep(NA, p),
                        "minsq", acut = acut)

  beta = c(-0.3, -0.1, 3)
  n = 10
  set.seed(134)
  utabl <- MCMCpack::rdirichlet(n, beta+1)
  out <- cppad_closed(tapes$smotape, Y = utabl)

  hardcodedestimate <- dir_sqrt_minimah(utabl, acut)
  expect_equal(out$est, hardcodedestimate, ignore_attr = TRUE)
})


test_that("minsq weights match estimator2 for d = 4", {
  acut = 0.1
  p = 4
  tapes <- buildsmotape("sphere", "dirichlet",
                        rep(1/p, p), rep(NA, p),
                        "minsq", acut = acut)

  beta = c(-0.3, -0.1, -0.2, 3)
  n = 10
  set.seed(134)
  utabl <- MCMCpack::rdirichlet(n, beta+1)

  out <- cppad_closed(tapes$smotape, Y = utabl)
  hardcodedestimate <- dir_sqrt_minimah(utabl, acut)
  expect_equal(out$est, hardcodedestimate, ignore_attr = TRUE)
})

test_that("fixed beta[p] with minsq weights match true value", {
  acut = 0.1
  beta = c(-0.2, -0.1, 3)
  n = 10000
  set.seed(134)
  utabl <- MCMCpack::rdirichlet(n, beta+1)

  p = length(beta)
  tapes <- buildsmotape("sphere", "dirichlet",
                        rep(1/p, p), c(NA, NA, beta[3]),
                        "minsq", acut = acut)
  out <- cppad_closed(tapes$smotape, Y = utabl)

  expect_absdiff_lte_v(out$est, beta[-p], 3 * out$SE)
})


test_that("cppad-based Score2 estimate leads to a match for large number of observations", {
  p = 3
  tapes <- buildsmotape("simplex", "dirichlet",
                        rep(1/p, p), rep(NA, p),
                        "prodsq", acut = 0.1)

  beta = c(-0.3, -0.1, 3)
  n = 1000
  set.seed(134)
  utabl <- MCMCpack::rdirichlet(n, beta+1)
  out <- cppad_closed(tapes$smotape, Y = utabl)
  expect_absdiff_lte_v(out$est, beta, out$SE * 3)
})

test_that("Simplex calculations are historically consistent", {
  p = 3
  tapes <- buildsmotape("simplex", "dirichlet",
                        rep(1/p, p), rep(NA, p),
                        "prodsq", acut = 1)

  beta = c(-0.3, -0.1, 3)
  n = 10
  set.seed(134)
  utabl <- MCMCpack::rdirichlet(n, beta+1)

  expect_snapshot_value(smobj(tapes$smotape, beta + 0.5, utabl), style = "json2", tolerance = 1E-5)
  expect_snapshot_value(smobjgrad(tapes$smotape, beta + 0.5, utabl), style = "json2", tolerance = 1E-5)
})