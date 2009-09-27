// $Id$
(function ($) {
// START jQuery

Drupal.vbo = Drupal.vbo || {};
Drupal.vbo.action = Drupal.vbo.action || {};

Drupal.vbo.action.updateOperations = function(vid) {
  var options = "";
  if (Drupal.settings.vbo.action.views_operations[vid] == undefined) {
    options += "<option value=\"0\">" + Drupal.t("- No operation found in this view -") + "</option>";
  }
  else {
    options += "<option value=\"0\">" + Drupal.t("- Choose an operation -") + "</option>";
    $.each(Drupal.settings.vbo.action.views_operations[vid], function(value, text) {
      options += "<option value=\"" + value + "\">" + text + "</option>\n";
    });
  }
  $("#edit-operation-callback").html(options);
}

Drupal.behaviors.vbo_action = function(context) {
  vid = $("#edit-view-vid").val();
  if (vid !=0) {
    operation = $("#edit-operation-callback").val();
    Drupal.vbo.action.updateOperations(vid);
    $("#edit-operation-callback").val(operation);
  }
}

// END jQuery
})(jQuery);

