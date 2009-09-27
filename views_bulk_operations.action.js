// $Id$
(function ($) {
// START jQuery

Drupal.vbo = Drupal.vbo || {};
Drupal.vbo.action = Drupal.vbo.action || {};

Drupal.vbo.action.updateOperations = function(vid) {
  var options = "";
  if (Drupal.settings.vbo.action.views_operations[vid] == undefined) {
    options += "<option value=\"0\">- No operation found in this view -</option>";
  }
  else $.each(Drupal.settings.vbo.action.views_operations[vid], function(value, text) {
    options += "<option value=\"" + value + "\">" + text + "</option>\n";
  });
  $("#edit-operation-callback").html(options);
}

// END jQuery
})(jQuery);

