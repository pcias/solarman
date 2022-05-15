# Hello, world!
#
# This is an example function named 'hello'
# which prints 'Hello, world!'.
#
# You can learn more about package authoring with RStudio at:
#
#   http://r-pkgs.had.co.nz/
#
# Some useful keyboard shortcuts for package authoring:
#
#   Install Package:           'Cmd + Shift + B'
#   Check Package:             'Cmd + Shift + E'
#   Test Package:              'Cmd + Shift + T'

#' Get authorization token
#'
#' @param appId
#' @param appSecret
#' @par
























...
#'
#' @return
#' @export
#'
#' @examples
token <- function(appId,appSecret,email,password,...) {
  # Setup
  dots <- list(...)
  ua   <- httr::user_agent("https://github.com/mailforlul")

  # Overrides
  dots$appId <- appId

  #Generate JSON body
  req_body = {}
  req_body$appSecret <- appSecret
  req_body$email <- email
  req_body$password <- digest::digest(password, algo=c("sha256"), serialize=F)

  # Generate URL
  url_params <- stringr::str_c(names(dots), dots, sep = "=", collapse = "&")
  url <- glue::glue("https://api.solarmanpv.com/account/v1.0/token?{url_params}")

  # call call
  response <- httr::POST(url = url, body = req_body, encode = "json" )

  httr::content(response)
}


#' List available "plants" (installations)
#'
#' @param token
#' @param ...
#'
#' @return
#' @export
#'
#' @examples
plantlist <-function(token,...) {
  #Generate JSON body
  req_body = {}
  req_body$a = "a"

  # Setup
  dots <- list(...)

  # Generate URL
  url_params <- stringr::str_c(names(dots), dots, sep = "=", collapse = "&")
  url <- glue::glue("https://api.solarmanpv.com/station/v1.0/list?{url_params}")


  # call call
  response <- httr::POST(url = url,
                         httr::add_headers(Authorization = paste("Bearer", token, sep=" ")),
                         body = req_body,
                         encode = "json" )


  httr::content(response)
}


#' Generic API call once you have token
#'
#' @param token
#' @param route
#' @param ...
#'
#' @return endpoint return
#' @export
#'
#' @examples
station_call <- function(token,route, ...) {
  dots <- list(...)
  req_body <- dots
  url <- glue::glue("https://api.solarmanpv.com/station/v1.0/{route}")

  # call call
  response <- httr::POST(url = url,
                         httr::add_headers(Authorization = paste("Bearer", token, sep=" ")),
                         body = req_body,
                         encode = "json" )

}
