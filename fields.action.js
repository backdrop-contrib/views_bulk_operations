// $Id$
(function ($) {
// START jQuery

Drupal.vbo = Drupal.vbo || {};
Drupal.vbo.fieldsAction = Drupal.vbo.fieldsAction || {};

Drupal.vbo.fieldsAction.updateToggler = function(toggler, direct) {
  var parent = $(toggler).parents('tr')[0];
  if ((toggler.checked && direct) || (!toggler.checked && !direct)) {
    $('.fields-action-togglable :input', parent).removeAttr('disabled');
  }
  else {
    $('.fields-action-togglable :input', parent).attr('disabled', true);
  }
}

Drupal.behaviors.vbo_fieldsAction = function(context) {
  $('.fields-action-toggler').click(function() {
    Drupal.vbo.fieldsAction.updateToggler(this, true);
  });

  $('th.select-all').click(function() {
    $('.fields-action-toggler').each(function() {
      Drupal.vbo.fieldsAction.updateToggler(this, false);
    });
  });
  
  // Disable all by default.
  $('.fields-action-togglable :input').attr('disabled', true);
}

// END jQuery
})(jQuery);

