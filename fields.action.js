// $Id$
(function ($) {
// START jQuery

$(document).ready(function() {
  $('.fields-action-toggler').click(function() {
    var parent = $(this).parents('tr')[0];
    if ($(this).is(':checked')) {
      $('.fields-action-togglable :input', parent).removeAttr('disabled');
    }
    else {
      $('.fields-action-togglable :input', parent).attr('disabled', true);
    }
  });
  
  // Disable all by default.
  $('.fields-action-togglable :input').attr('disabled', true);
});

// END jQuery
})(jQuery);

