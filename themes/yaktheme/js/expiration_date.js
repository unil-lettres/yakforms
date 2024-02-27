const expirationDateFormatOptions = { year: 'numeric', month: 'long', day: 'numeric' };

function changeExpirationDateSubtitleHTML() {
  // Get the input value from the element.
  let expiration_date_value = new Date($('#edit-field-form1-expiration-und-0-value-datepicker-popup-0')
    .val())

  // Define the anchor element and insert it into the subtitle.
  let expirationText = Drupal.t('Will expire on')
    + " "
    + expiration_date_value.toLocaleDateString(undefined, expirationDateFormatOptions)
  $('#expiration-date-subtitle')
    .text(expirationText)
}

$(window).on('load', function() {
  // Define subtitle element and append it to the page title.
  let anchor_elem = $("<a></a")
    .attr('id', 'expiration-date-subtitle')
    .attr('href', '#field-form1-expiration-add-more-wrapper')
  $('#page-title').after(anchor_elem)

  // Set the initial expiration date as subtitle.
  changeExpirationDateSubtitleHTML()

  // Add event listener to the input to react to user action.
  $('#edit-field-form1-expiration-und-0-value-datepicker-popup-0')
    .change(changeExpirationDateSubtitleHTML)
})
