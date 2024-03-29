(function ($) {
  // Polyfill for jQuery less than 1.6.
  if (typeof $.fn.prop != 'function') {
    jQuery.fn.extend({
      prop: jQuery.fn.attr
    });
  }

  Backdrop.behaviors.vbo = {
    attach: function(context) {
      $('.vbo-views-form', context).each(function() {
        Backdrop.vbo.initTableBehaviors(this);
        Backdrop.vbo.initGenericBehaviors(this);
        Backdrop.vbo.toggleButtonsState(this);
      });
    }
  }

  Backdrop.vbo = Backdrop.vbo || {};
  Backdrop.vbo.initTableBehaviors = function(form) {
    // If the table is not grouped, "Select all on this page / all pages"
    // markup gets inserted below the table header.
    var selectAllMarkup = $('.vbo-table-select-all-markup', form);
    if (selectAllMarkup.length) {
      $('.views-table > tbody', form).prepend('<tr class="views-table-row-select-all even">></tr>');
      var colspan = $('table th', form).length;
      $('.views-table-row-select-all', form).html('<td colspan="' + colspan + '">' + selectAllMarkup.html() + '</td>');

      $('.vbo-table-select-all-pages', form).on('click', function() {
        Backdrop.vbo.tableSelectAllPages(form);
        return false;
      });
      $('.vbo-table-select-this-page', form).on('click', function() {
        Backdrop.vbo.tableSelectThisPage(form);
        return false;
      });
    }

    $('.vbo-table-select-all', form).show();
    // This is the "select all" checkbox in (each) table header.
    $('input.vbo-table-select-all', form).on('click', function() {
      var table = $(this).closest('table:not(.sticky-header)')[0];
      $('.vbo-select:not(:disabled)', table).prop('checked', this.checked);
      Backdrop.vbo.toggleButtonsState(form);

      // Toggle the visibility of the "select all" row (if any).
      if (this.checked) {
        $('.views-table-row-select-all', table).show();
      }
      else {
        $('.views-table-row-select-all', table).hide();
        // Disable "select all across pages".
        Backdrop.vbo.tableSelectThisPage(form);
      }
    });

    // Set up the ability to click anywhere on the row to select it.
    if (Backdrop.settings.vbo.row_clickable) {
      $('.views-table tbody tr', form).on('click', function(event) {
        var tagName = event.target.tagName.toLowerCase();
        if (tagName != 'input' && tagName != 'a' && tagName != 'label') {
          $('.vbo-select:not(:disabled)', this).each(function() {
            // Always return true for radios, you cannot de-select a radio by clicking on it,
            // it should be the same when clicking on a row.
            this.checked = $(this).is(':radio') ? true : !this.checked;
            $(this).trigger('change');
          });
        }
      });
    }
  }

  Backdrop.vbo.tableSelectAllPages = function(form) {
    $('.vbo-table-this-page', form).hide();
    $('.vbo-table-all-pages', form).show();
    // Modify the value of the hidden form field.
    $('.select-all-rows', form).val('1');
  }
  Backdrop.vbo.tableSelectThisPage = function(form) {
    $('.vbo-table-all-pages', form).hide();
    $('.vbo-table-this-page', form).show();
    // Modify the value of the hidden form field.
    $('.select-all-rows', form).val('0');
  }

  Backdrop.vbo.initGenericBehaviors = function(form) {
    // Show the "select all" fieldset.
    $('.vbo-select-all-markup', form).show();

    $('.vbo-select-this-page', form).on('click', function() {
      $('.vbo-select', form).prop('checked', this.checked);
      Backdrop.vbo.toggleButtonsState(form);
      $('.vbo-select-all-pages', form).prop('checked', false);

      // Toggle the "select all" checkbox in grouped tables (if any).
      $('.vbo-table-select-all', form).prop('checked', this.checked);
    });
    $('.vbo-select-all-pages', form).on('click', function() {
      $('.vbo-select', form).prop('checked', this.checked);
      Backdrop.vbo.toggleButtonsState(form);
      $('.vbo-select-this-page', form).prop('checked', false);

      // Toggle the "select all" checkbox in grouped tables (if any).
      $('.vbo-table-select-all', form).prop('checked', this.checked);

      // Modify the value of the hidden form field.
      $('.select-all-rows', form).val(this.checked);
    });

    // Toggle submit buttons' "disabled" states with the state of the operation
    // selectbox.
    $('select[name="operation"]', form).on('change', function () {
      Backdrop.vbo.toggleButtonsState(form);
    });

    // Handle a "change" event originating either from a row click or an actual checkbox click.
    $('.vbo-select', form).on('change', function() {
      // If a checkbox was deselected, uncheck any "select all" checkboxes.
      if (!this.checked) {
        $('.vbo-select-this-page', form).prop('checked', false);
        $('.vbo-select-all-pages', form).prop('checked', false);
        // Modify the value of the hidden form field.
        $('.select-all-rows', form).val('0')

        var table = $(this).closest('table')[0];
        if (table) {
          // Uncheck the "select all" checkbox in the table header.
          $('.vbo-table-select-all', table).prop('checked', false);

          // If there's a "select all" row, hide it.
          if ($('.vbo-table-select-this-page', table).length) {
            $('.views-table-row-select-all', table).hide();
            // Disable "select all across pages".
            Backdrop.vbo.tableSelectThisPage(form);
          }
        }
      }

      Backdrop.vbo.toggleButtonsState(form);
    });
  }

  Backdrop.vbo.toggleButtonsState = function(form) {
    // If no rows are checked, disable any form submit actions.
    var selectbox = $('select[name="operation"]', form);
    var checkedCheckboxes = $('.vbo-select:checked', form);
    // The .vbo-prevent-toggle CSS class is added to buttons to prevent toggling
    // between disabled and enabled. For example the case of an 'add' button.
    var buttons = $('[id^="edit-select"] [type="submit"]:not(.vbo-prevent-toggle)', form);

    if (selectbox.length) {
      var has_selection = checkedCheckboxes.length && selectbox.val() !== '0';
      buttons.prop('disabled', !has_selection);
    }
    else {
      buttons.prop('disabled', !checkedCheckboxes.length);
    }
  };

})(jQuery);
