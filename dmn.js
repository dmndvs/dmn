(function($) {
    /**
     * Add placeholder to input textfield
     * @type {{attach: Drupal.behaviors.dmn.attach}}
     */
    Drupal.behaviors.dmn = {
        attach: function () {
            jQuery("#dmn-fieldset input[name=dmn_fname]").attr('placeholder','First Name');
            jQuery("#dmn-fieldset input[name=dmn_lname]").attr('placeholder','Last Name');
        }
    };
    /**
     * @param ajax
     * @param response
     * @param status
     */
    Drupal.ajax.prototype.commands.dmn_rebuild = function(ajax, response, status){

            jQuery('#' + response.elementId).replaceWith('<div>hello, ' + response.fname + ' ' + response.lname + '</div>');

    };
    /**
     * Redirect page to 'response'
     *
     * @param ajax
     * @param response
     * @param status
     */
    Drupal.ajax.prototype.commands.dmn_redirect = function(ajax, response, status){
        window.location = response.topath;
    };
    /**
     * Example from web.
     * see url = /dmn/view2
     *
     * @param ajax
     * @param response
     * @param status
     */
    Drupal.ajax.prototype.commands.special_effects = function(ajax, response, status){
        jQuery('#' + response.elementId).hide().text(response.name).fadeIn(response.duration, function() {
            var fontSize = parseInt($(this).css('font-size'));
            jQuery(this).animate({'font-size': (response.amplify * fontSize) + "px"}, 5000, function() {
                jQuery(this).animate({'font-size': fontSize + "px"}, 5000);
            });
        });
    };
}(jQuery));







